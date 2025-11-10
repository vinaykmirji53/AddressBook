#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "populate.h"
#include <ctype.h>
#include "file.h"

void listContacts(AddressBook *addressBook, int sortCriteria) 
{
  printf("%-5s %-30s %-30s %-30s\n","No","Name","Phone","Email");  // Print table header

  if (addressBook->contactCount == 0)  // Check if address book is empty
  {
    printf("No contacts available.\n");  
    return;  // Exit function if no contacts
  }

  if(sortCriteria == 4)// If no sorting, just print contacts
  {  
    for(int i = 0; i < addressBook->contactCount; i++)
    { 
      printf("%-5d %-30s %-30s %-30s\n", i+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);  // Print each contact
    }
    return;  // Done printing unsorted
  }

  // Bubble sort contacts based on sortCriteria
  for(int i=0; i < addressBook->contactCount - 1; i++) 
  {
    for(int j=0; j < addressBook->contactCount - i - 1; j++) 
    {
      int cmp=0;  // Comparison result
      if(sortCriteria == 1)
        cmp = strcmp(addressBook->contacts[j].name, addressBook->contacts[j+1].name);  // Compare names
      else if(sortCriteria == 2)
        cmp = strcmp(addressBook->contacts[j].phone, addressBook->contacts[j+1].phone);  // Compare phones
      else if(sortCriteria == 3)
        cmp = strcmp(addressBook->contacts[j].email, addressBook->contacts[j+1].email);  // Compare emails

      if(cmp > 0) // Swap if current > next
      {
        Contact temp = addressBook->contacts[j];
        addressBook->contacts[j] = addressBook->contacts[j+1];
        addressBook->contacts[j+1] = temp;
      }
    }
  }

  // Print sorted contacts
  for(int i = 0; i < addressBook->contactCount; i++) 
  {
    printf("%-5d %-30s %-30s %-30s\n", i+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
  }
}


void initialize(AddressBook *addressBook)
{
  addressBook->contactCount = 0; // '->' is taken bcs addressBook is a pointer
  //populateAddressBook(addressBook); 
  // Load contacts from file during initialization (After files)
  loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) 
{
  saveContactsToFile(addressBook); // Save contacts to file
  exit(EXIT_SUCCESS); // Exit the program
}

// Create a new contact with validation
void createContact(AddressBook *addressBook)
{
  char name[50], number[50], email[50];  // Temporary buffers for input

  if (addressBook->contactCount >= 100) // Check if address book is full
  { printf("AddressBook is full!\n");
    return;
  }

  // Input and validate name
  do {
    printf("1.Name: ");
    scanf(" %[^\n]", name);  // Read full line including spaces
  } while (!validateName(name, addressBook));

  // Input and validate phone
  do {
    printf("2.Phone: ");
    scanf(" %[^\n]", number);
  } while (!validatePhoneNumber(number, addressBook));

  // Input and validate email
  do {
    printf("3.Email: ");
    scanf(" %[^\n]", email);
  } while (!validateEmail(email, addressBook));

  // Copy validated data into AddressBook
  strcpy(addressBook->contacts[addressBook->contactCount].name, name);
  strcpy(addressBook->contacts[addressBook->contactCount].phone, number);
  strcpy(addressBook->contacts[addressBook->contactCount].email, email);

  addressBook->contactCount++;  // Increment contact count
  printf("Contact added successfully!\n");
}


// Search for a contact by name, phone, or email
int searchContact(AddressBook *addressBook) 
{
  if (addressBook->contactCount == 0) // Check if empty
  {  
    printf("No contacts available.\n");
    return -1;  // Not found
  }

  int choice;
  char search[30];

  printf("select any one:\n1. search by Name\n2. search by Phone\n3. search by Email\n");
  printf("Enter choice: ");
  scanf("%d", &choice);

  if(choice < 1 || choice > 3) // Invalid input
  {
    printf("Invalid choice!\n");
    return -1;
  }

  printf("Enter search value: ");
  scanf(" %[^\n]", search);

  for (int i = 0; i < addressBook->contactCount; i++)
  {
    int count = 0;
    switch(choice)
    {
      case 1:
      count = (strcasecmp(addressBook->contacts[i].name, search) == 0);  // Compare name
      break;

      case 2:
      count = (strcasecmp(addressBook->contacts[i].phone, search) == 0);  // Compare phone
      break;

      case 3:
      count = (strcasecmp(addressBook->contacts[i].email, search) == 0);  // Compare email
      break;
    }

    if(count)  // Found contact
    {
      printf("Found Contact:\n");
      printf("%-5d %-30s %-30s %-30s\n", i+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
      return i;  // Return index
    }
  }

  printf("Contact not found.\n");  // Not found
  return -1;
}


// Edit an existing contact
void editContact(AddressBook *addressBook)
{
  int index = searchContact(addressBook);  // Find contact index
  if (index < 0)
  return;  // Exit if not found

  int option;
  char newValue[50];

  printf("\nEdit Menu\n1.Edit the Name\n2.Edit the Phone\n3.Edit the Email\nChoose any one option : ");
  scanf("%d", &option);

  switch(option) 
  {
    case 1:  // Edit name
    printf("Enter the new name: ");
    scanf(" %[^\n]", newValue);
    if(!validateName(newValue, addressBook)) 
    {
      printf("Update cancelled due to invalid name.\n");
      return;
    }
    strcpy(addressBook->contacts[index].name, newValue);
    break;

    case 2:  // Edit phone
    printf("Enter the new number : ");
    scanf(" %[^\n]", newValue);
    if(!validatePhoneNumber(newValue, addressBook)) 
    {
      printf("Update cancelled due to invalid phone number.\n");
      return;
    }
    strcpy(addressBook->contacts[index].phone, newValue);
    break;

    case 3:  // Edit email
    printf("Enter the new email : ");
    scanf(" %[^\n]", newValue);
    if(!validateEmail(newValue, addressBook)) 
    {
      printf("Update cancelled due to invalid email.\n");
      return;
    }
    strcpy(addressBook->contacts[index].email, newValue);
    break;

    default:
    printf("Invalid option.\n");
    return;
  }

  printf("Contact updated successfully!\n");
}


// Delete a contact by index
void deleteContact(AddressBook *addressBook)
{
  if(addressBook->contactCount == 0) // No contacts
  {  
    printf("No contacts available to delete.\n");
    return;
  }

  char search[30];
  printf("Enter name/phone/email to delete: ");
  scanf(" %[^\n]", search);

  int foundIndexes[100], foundCount = 0;

  for(int i = 0; i < addressBook->contactCount; i++) 
  {
    if(strcasecmp(addressBook->contacts[i].name, search) == 0 ||
    strcasecmp(addressBook->contacts[i].phone, search) == 0 ||
    strcasecmp(addressBook->contacts[i].email, search) == 0) 
    {
      printf("%d. %s %s %s\n", i+1, addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
      foundIndexes[foundCount++] = i;  // Save matching indexes
    }
  }

  if(foundCount == 0) // No match
  { 
    printf("No matching contact found.\n");
    return;
  }

  int choice;
  if(foundCount > 1)  // Multiple matches
  {
    printf("Multiple matches found.\n Enter the index to delete: ");
    scanf("%d", &choice);
    choice--;  // Convert to array index
  }
  else 
  {
    choice = foundIndexes[0];  // Only one match
  }

  // Delete contact by shifting array
  Contact deleted = addressBook->contacts[choice];
  for(int i = choice; i < addressBook->contactCount - 1; i++)
  addressBook->contacts[i] = addressBook->contacts[i+1];

  addressBook->contactCount--;  // Decrement count
  printf("Deleted contact: %s\n", deleted.name);
}

// Validate that name contains only alphabets and spaces
int validateName(char *name, AddressBook *addressBook)
{
  for(int i=0; name[i]; i++) 
  {
    if(!isalpha(name[i]) && name[i] != ' ')  // Only alphabets and spaces allowed
    {        
      printf("Entered name is Invalid! \nName contains only alphabets and spaces.\n");
      return 0;
    }
  }
  return 1;  // Valid name
}

// Validate phone number is 10 digits and unique
int validatePhoneNumber(char *phone, AddressBook *addressBook)
{
  int len = strlen(phone);
  if(len != 10)  // Must be 10 digits
  {      
    printf("Phone number must contain only 10 digits.\n");
    return 0;
  }

  for(int i=0; i<len; i++)
  if(!isdigit(phone[i])) // Only digits allowed
  {          
    printf("Phone number must contain only digits.\n");
    return 0;
  }

  for(int j=0; j<addressBook->contactCount; j++)  // Check duplicates
  if(strcmp(addressBook->contacts[j].phone, phone) == 0) 
  {
    printf("This phone number already exists.\n");
    return 0;
  }
  return 1;  // Valid phone
}

// Validate email format and uniqueness
int validateEmail(char *mail, AddressBook *addressBook)
{
  if(!strchr(mail,'@') || !strchr(mail,'.'))  // Must contain @ and .
  {    
    printf("Email must contain '@' and '.'\n");
    return 0;
  }

  if(mail[0] == '@' || mail[0] == '.')  // Cannot start with @ or .
  {    
    printf("Email cannot start with '@' or '.'\n");
    return 0;
  }

  for(int j=0; j<addressBook->contactCount; j++)  // Check duplicates
  if(strcmp(addressBook->contacts[j].email, mail) == 0) 
  {
    printf("This email already exists.\n");
    return 0;
  }
  return 1;  // Valid email
}
