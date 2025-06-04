#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct items {
    char item[50];
    float price;
    int qty;
};

struct orders {
    char customer[50];
    char date[50];
    int numOfItems;
    struct items itm[100];
};

// BILL GENERATION FUNCTIONS
void generateBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t    ADV. Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\tQty\t\tTotal\n");
    printf("---------------------------------------\n");
}

void generateBillBody(char item[30], int qty, float price) {
    printf("%s\t\t%d\t\t%.2f\n", item, qty, qty * price);
}

void generateBillFooter(float total) {
    float dis = 0.1 * total;
    float netTotal = total - dis;
    float cgst = 0.09 * netTotal;
    float grandTotal = netTotal + (2 * cgst);

    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f\n", total);
    printf("Discount @10%%\t\t\t%.2f\n", dis);
    printf("Net Total\t\t\t%.2f\n", netTotal);
    printf("CGST @9%%\t\t\t%.2f\n", cgst);
    printf("SGST @9%%\t\t\t%.2f\n", cgst);
    printf("---------------------------------------\n");
    printf("Grand Total\t\t\t%.2f\n", grandTotal);
    printf("---------------------------------------\n");
}

// TERMINAL MODE (full interactive)
int terminalMode() {
    int opt, n;
    struct orders ord;
    struct orders order;
    char saveBill = 'y', contFlag = 'y';
    char name[50];
    FILE *fp;

    while (contFlag == 'y') {
        system("clear"); // or "cls" on Windows
        float total = 0;
        int invoiceFound = 0;

        printf("\t============ADV. RESTAURANT============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &opt);
        fgetc(stdin);  // consume newline after scanf

        switch (opt) {
            case 1:
                system("clear");
                printf("\nPlease enter the name of the customer:\t");
                fgets(ord.customer, 50, stdin);
                ord.customer[strcspn(ord.customer, "\n")] = 0; // remove newline
                strcpy(ord.date, __DATE__);

                printf("\nPlease enter the number of items:\t");
                scanf("%d", &n);
                ord.numOfItems = n;
                fgetc(stdin); // consume newline

                total = 0;
                for (int i = 0; i < n; i++) {
                    printf("\nItem %d name:\t", i + 1);
                    fgets(ord.itm[i].item, 50, stdin);
                    ord.itm[i].item[strcspn(ord.itm[i].item, "\n")] = 0;

                    printf("Quantity:\t");
                    scanf("%d", &ord.itm[i].qty);

                    printf("Unit Price:\t");
                    scanf("%f", &ord.itm[i].price);
                    fgetc(stdin);

                    total += ord.itm[i].qty * ord.itm[i].price;
                }

                generateBillHeader(ord.customer, ord.date);
                for (int i = 0; i < ord.numOfItems; i++) {
                    generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
                }
                generateBillFooter(total);

                printf("\nDo you want to save the invoice [y/n]:\t");
                scanf("%c", &saveBill);
                fgetc(stdin);

                if (saveBill == 'y' || saveBill == 'Y') {
                    fp = fopen("RestaurantBill.dat", "a+");
                    if (fp == NULL) {
                        printf("Error opening file!\n");
                        break;
                    }
                    fwrite(&ord, sizeof(struct orders), 1, fp);
                    fclose(fp);
                    printf("\nSuccessfully saved\n");
                }
                break;

            case 2:
                system("clear");
                fp = fopen("RestaurantBill.dat", "r");
                if (fp == NULL) {
                    printf("No invoices found.\n");
                    break;
                }
                printf("\n  ***** Your Previous Invoices *****\n");
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    float tot = 0;
                    generateBillHeader(order.customer, order.date);
                    for (int i = 0; i < order.numOfItems; i++) {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }
                    generateBillFooter(tot);
                }
                fclose(fp);
                break;

            case 3:
                printf("Enter the name of the customer:\t");
                fgets(name, 50, stdin);
                name[strcspn(name, "\n")] = 0;

                system("clear");
                fp = fopen("RestaurantBill.dat", "r");
                if (fp == NULL) {
                    printf("No invoices found.\n");
                    break;
                }

                printf("\t***** Invoice of %s *****\n", name);
                invoiceFound = 0;
                while (fread(&order, sizeof(struct orders), 1, fp)) {
                    if (strcmp(order.customer, name) == 0) {
                        float tot = 0;
                        generateBillHeader(order.customer, order.date);
                        for (int i = 0; i < order.numOfItems; i++) {
                            generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                            tot += order.itm[i].qty * order.itm[i].price;
                        }
                        generateBillFooter(tot);
                        invoiceFound = 1;
                    }
                }
                if (!invoiceFound) {
                    printf("Sorry, invoice for %s does not exist.\n", name);
                }
                fclose(fp);
                break;

            case 4:
                printf("\n\t\t Bye Bye :)\n\n");
                exit(0);

            default:
                printf("Sorry, invalid option.\n");
                break;
        }

        printf("\nDo you want to perform another operation? [y/n]:\t");
        scanf("%c", &contFlag);
        fgetc(stdin);
    }

    printf("\n\t\t Bye Bye :)\n\n");
    return 0;
}

// WEB MODE: Simple single invoice from stdin (no file ops, no menu)
int webMode() {
    struct orders ord;
    float total = 0;
    int n;

    // Read customer name
    fgets(ord.customer, 50, stdin);
    ord.customer[strcspn(ord.customer, "\n")] = 0;  // Remove newline

    // Read number of items
    scanf("%d", &n);
    ord.numOfItems = n;

    getchar(); // consume newline

    for (int i = 0; i < n; i++) {
        fgets(ord.itm[i].item, 50, stdin);
        ord.itm[i].item[strcspn(ord.itm[i].item, "\n")] = 0;

        scanf("%d", &ord.itm[i].qty);
        scanf("%f", &ord.itm[i].price);
        getchar(); // consume newline after price

        total += ord.itm[i].qty * ord.itm[i].price;
    }

    // Set date from __DATE__
    strcpy(ord.date, __DATE__);

    generateBillHeader(ord.customer, ord.date);
    for (int i = 0; i < ord.numOfItems; i++) {
        generateBillBody(ord.itm[i].item, ord.itm[i].qty, ord.itm[i].price);
    }
    generateBillFooter(total);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--web") == 0) {
        return webMode();
    } else {
        return terminalMode();
    }
}
