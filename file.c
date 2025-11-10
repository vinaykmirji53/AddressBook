#include <stdio.h>
#include "file.h"

// Save contacts to a file
void saveContactsToFile(AddressBook *addressBook) {
    FILE *fptr = fopen("contact.csv", "w");
    if (fptr == NULL) {
        printf("Unable to save contacts to file.\n");
        return;
    }
    fprintf(fptr, "%d\n", addressBook->contactCount);

    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
    printf("Contacts saved successfully.\n");
}

// Load contacts from a file
void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fptr = fopen("contact.csv", "r");
    if (fptr == NULL) {
        printf("Unable to open the file.\n");
        return;
    }
    fscanf(fptr, "%d\n", &addressBook->contactCount);
    
    for (int i = 0; i < addressBook->contactCount; i++) {
        fscanf(fptr,"%49[^,],%14[^,],%49[^\n]\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fptr);
    printf("Contacts loaded successfully.\n");
}


