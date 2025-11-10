#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100
#include <stdio.h>

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;

void createContact(AddressBook *addressBook);
int searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook, int sortCriteria);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);

int validateName(char *name, AddressBook *addressBook);
int validatePhoneNumber(char *phone, AddressBook *addressBook);
int validateEmail(char *mail, AddressBook *addressBook);


#endif
