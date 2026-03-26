#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int log_id;
    char type[10];
    char item_id[20];
    int quantity;
}  AuditLog;

typedef struct {
    char id[20];
    char name[50];
    int quantity;
    double price;
}  Item;

Item inventory;
AuditLog logsaudit;
int itemCount = 0;
int logCount = 0;
int globalLogId =1;

int findItemIndex(char *id) {
    for (int i = 0; i < itemCount;  i++) {
        if (strcmmp(inventory[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

void addLog(char *type, char *item_id,int qty) {
    logs[logCount].log_id = globalLogId++;
    strcpy(logs[logCount].type, type);
    strcpy(logs[logCount].item_id, item_id);
    logs[logCount].quantity = qty;
    logCount++;

}

int main() {
    char input[256];

    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "---") == 0) break;
        
          char *command = strtok(input, "#");
          if (command == NULL) continue;

          if (strcmp(command,"receive") == 0) {
            char *id = strtok(NULL, "#");
            char *name = strtok(NULL, "#");
            char *qtyStr = strtok(NULL, "#");
            char *priceStr = strtok(NULL, "#");

            if (id && name && qtyStr && priceStr) {
                int qty = atoi(qtyStr);
                double price = atof(priceStr);

                strcpy(inventory[itemCount].id, id);
                strcpy(inventory[itemCount].name, name);
                inventory[itemCount].quantity = qty;
                inventory[itemCount].price = price;

                addLog("receive", id,qty);
                itemCount++;
            }
        }
          else if (strcmp(command,"ship") == 0) {
            char *id = strtok(NULL, "#");
            char *qtyStr = strtok(NULL, "#");

            if (id && qtyStr) {
                int qty = atoi(qtyStr);
                int idx = findItemIndex(id);

                if (idx != -1 && qty > 0) {
                    inventory[idx].quantity += qty;
                    addLog("restock", id, qty);

                }
            }
        }
        
        else if (strcmp(command, "report") == 0) {
           for (int i = 0; i < itemCount; i++) {
               printf("%s|%s|%d|%.1f\n", inventory[i].id, inventory[i].name,
                inventory[i].quantity, inventory[i].price);
           }

        }
        else if (strcmp(command, "audit") == 0) {
            char *id = strtok(NULL, "#");
            if (id) {
                int idx = findItemIndex(id);
                if (idx != -1) {
                    printf("%s|%s|%d|%.1f\n", inventory[idx].id, inventory[idx].name,
                    inventory[idx].quantity, inventory[idx].price);

                    for (int j = 0; j <logCount; j++) {
                        if (strcmp(logs[j].item_id, id) == 0) {
                            printf("%d|%s|%d\n", logs[j].log_id, logs[j].type, logs[j].quantity);
                        }
                    }
                }
            }
        }
    }

    return 0;
}