#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_CATEGORIES 50
#define MAX_ITEMS_PER_CATEGORY 50

// Global variables to hold category and item details
char categoryNames[MAX_CATEGORIES][MAX_NAME_LENGTH];                     // Array to store category names
char itemNames[MAX_CATEGORIES][MAX_ITEMS_PER_CATEGORY][MAX_NAME_LENGTH]; // Array to store item names per category
int stockLevels[MAX_CATEGORIES][MAX_ITEMS_PER_CATEGORY];                 // Array to store stock levels of items per category
int reorderPoints[MAX_CATEGORIES][MAX_ITEMS_PER_CATEGORY];               // Array to store reorder points of items per category
float prices[MAX_CATEGORIES][MAX_ITEMS_PER_CATEGORY];                    // Array to store prices of items per category
int categoryCount = 0;                                                   // Number of categories added
int itemCounts[MAX_CATEGORIES] = {0};                                    // Item count per category

// Function to display categories and their items in a table-like format
void displayCategories();
void displayItems();
void addCategory();
void addItemCategory();
void adjustStock();
void searchItems();

// Add this new function before main()
void checkLowStock() {
    int lowStockFound = 0;
    
    // Check if there are any items
    for (int i = 0; i < categoryCount; i++) {
        for (int j = 0; j < itemCounts[i]; j++) {
            if (stockLevels[i][j] <= reorderPoints[i][j]) {
                lowStockFound = 1;
                break;
            }
        }
        if (lowStockFound) break;
    }

    if (categoryCount == 0) {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║         STOCK LEVEL STATUS             ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║  No items in inventory yet             ║\n");
        printf("╚════════════════════════════════════════╝\n");
    } else {
        // Print stock status
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║         STOCK LEVEL STATUS             ║\n");
        printf("╠════════════════════════════════════════╣\n");
        if (!lowStockFound) {
            printf("║  All items are above reorder points    ║\n");
        } else {
            printf("║  ⚠️  LOW STOCK WARNING  ⚠️             ║\n");
            printf("║  Some items are below reorder points!  ║\n");
        }
        printf("╠════════════════════════════════════════╣\n");
        printf("║         ITEMS PER CATEGORY             ║\n");
        printf("╠════════════════════════════════════════╣\n");
        
        // Print item counts for each category
        for (int i = 0; i < categoryCount; i++) {
            printf("║  %-20s: %3d items       ║\n", categoryNames[i], itemCounts[i]);
        }
        printf("╚════════════════════════════════════════╝\n");
    }
}

// Main program to interact with the user
int main()
{
    int choice;

    do
    {
        // Show stock status at the start of each menu loop
        checkLowStock();

        printf("\n╔════════════════════════════════════════════════════════════════╗\n");
        printf("║             WELCOME TO SARI-SARI STORE INVENTORY               ║\n");
        printf("║                         TRACKER                                ║\n");
        printf("╠════════════════════════════════════════════════════════════════╣\n");
        printf("║                                                                ║\n");
        printf("║   [1] Add Category                                             ║\n");
        printf("║   [2] Add Item                                                 ║\n");
        printf("║   [3] Add or Deduct Stock                                      ║\n");
        printf("║   [4] View Items                                               ║\n");
        printf("║   [5] Search Items                                             ║\n");
        printf("║   [6] Exit                                                     ║\n");
        printf("║                                                                ║\n");
        printf("╚════════════════════════════════════════════════════════════════╝\n");
        printf("\nEnter Choice: ");

        // Ensure valid input by reading the user input as a string first
        if (scanf("%d", &choice) != 1)
        {
            // Clear the invalid input
            while (getchar() != '\n')
                ; // Clear the input buffer
            printf("\nInvalid input. Please enter a valid number.\n");
            continue; // Skip the rest of the loop and prompt again
        }

        switch (choice)
        {
        case 1:
            addCategory();
            break;
        case 2:
            addItemCategory();
            break;
        case 3:
            adjustStock();
            break;
        case 4:
            displayItems();
            break;
        case 5:
            searchItems();
            break;
        case 6:
            printf("\nThank you for using the Inventory Tracker! Exiting now...\n");
            break;
        default:
            printf("\nInvalid Input. Please enter a valid choice.\n");
            break;
        }
    } while (choice != 6); // Repeat the menu until the user chooses to exit

    return 0;
}
// Function to add a category
void addCategory()
{
    if (categoryCount >= MAX_CATEGORIES) {
        printf("Maximum categories reached.\n");
        return;
    }

    char tempName[MAX_NAME_LENGTH];
    char choice;
    
    do {
        printf("\nEnter category name (or type 'exit' to return to main menu): ");
        
        // Clear input buffer before reading
        while (getchar() != '\n');
        
        if (fgets(tempName, MAX_NAME_LENGTH, stdin) == NULL) {
            printf("Error reading input.\n");
            return;
        }
        
        // Remove trailing newline
        tempName[strcspn(tempName, "\n")] = 0;
        
        // Check if user wants to exit
        if (strcmp(tempName, "exit") == 0) {
            printf("Returning to main menu...\n");
            return;
        }
        
        // Validate empty input
        if (strlen(tempName) == 0) {
            printf("Category name cannot be empty. Try again? (y/n): ");
            scanf(" %c", &choice);
            if (choice != 'y' && choice != 'Y') {
                return;
            }
            continue;
        }
        
        // Validate if category contains only spaces
        int onlySpaces = 1;
        for (int i = 0; tempName[i]; i++) {
            if (tempName[i] != ' ' && tempName[i] != '\t') {
                onlySpaces = 0;
                break;
            }
        }
        
        if (onlySpaces) {
            printf("Category name cannot contain only spaces. Try again? (y/n): ");
            scanf(" %c", &choice);
            if (choice != 'y' && choice != 'Y') {
                return;
            }
            continue;
        }
        
        // Check if category name already exists
        int categoryExists = 0;
        for (int i = 0; i < categoryCount; i++) {
            if (strcmp(tempName, categoryNames[i]) == 0) {
                printf("Category already exists. Try again? (y/n): ");
                scanf(" %c", &choice);
                if (choice != 'y' && choice != 'Y') {
                    return;
                }
                categoryExists = 1;
                break;
            }
        }
        
        if (categoryExists) {
            continue;
        }
        
        // If we reach here, the input is valid
        strcpy(categoryNames[categoryCount], tempName);
        categoryCount++;
        printf("\nCategory '%s' added successfully!\n", tempName);
        displayCategories();
        
        // Ask if user wants to add another category
        printf("\nDo you want to add another category? (y/n): ");
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            printf("Returning to main menu...\n");
            return;
        }
        
    } while (1);
}

// Function to add an item to a selected category
void addItemCategory()
{
    if (categoryCount > 0)
    {
        int categoryChoice;
        char choice;
        
        do {
            // Display categories for user to choose
            printf("\nChoose a category to add item to (or enter 0 to exit):\n");
            for (int i = 0; i < categoryCount; i++)
            {
                printf("%d. %s\n", i + 1, categoryNames[i]);
            }

            printf("\nEnter category number: ");
            if (scanf("%d", &categoryChoice) != 1) {
                while (getchar() != '\n');
                printf("\nInvalid input. Please enter a number.\n");
                continue;
            }

            // Check if user wants to exit
            if (categoryChoice == 0) {
                printf("Returning to main menu...\n");
                return;
            }

            // Validate category choice
            if (categoryChoice < 1 || categoryChoice > categoryCount) {
                printf("\nInvalid category number. Try again? (y/n): ");
                scanf(" %c", &choice);
                while (getchar() != '\n');
                
                if (choice != 'y' && choice != 'Y') {
                    printf("Returning to main menu...\n");
                    return;
                }
                continue;
            }

            break; // Valid category selected
        } while (1);

        categoryChoice--; // Adjust for 0-based indexing

        if (itemCounts[categoryChoice] < MAX_ITEMS_PER_CATEGORY)
        {
            char tempName[MAX_NAME_LENGTH];
            int tempStock, tempReorder;
            float tempPrice;
            char choice;

            // Item name input loop
            do {
                printf("\nEnter item name: ");
                while (getchar() != '\n'); // Clear buffer
                if (scanf(" %49[^\n]", tempName) != 1) {
                    printf("Invalid item name. Try again? (y/n): ");
                    scanf(" %c", &choice);
                    if (choice != 'y' && choice != 'Y') {
                        return;
                    }
                    continue;
                }

                // Check for existing item
                int itemExists = 0;
                for (int i = 0; i < itemCounts[categoryChoice]; i++) {
                    if (strcmp(tempName, itemNames[categoryChoice][i]) == 0) {
                        printf("Item already exists in this category. Try again? (y/n): ");
                        scanf(" %c", &choice);
                        if (choice != 'y' && choice != 'Y') {
                            return;
                        }
                        itemExists = 1;
                        break;
                    }
                }
                if (itemExists) continue;
                break;
            } while (1);

            // Stock level input loop
            do {
                printf("Enter stock level: ");
                if (scanf("%d", &tempStock) != 1 || tempStock < 0) {
                    while (getchar() != '\n');
                    printf("Invalid stock level. Try again? (y/n): ");
                    scanf(" %c", &choice);
                    if (choice != 'y' && choice != 'Y') {
                        return;
                    }
                    continue;
                }
                break;
            } while (1);

            // Reorder point input loop
            do {
                printf("Enter reorder point: ");
                if (scanf("%d", &tempReorder) != 1 || tempReorder < 0) {
                    while (getchar() != '\n');
                    printf("Invalid reorder point. Try again? (y/n): ");
                    scanf(" %c", &choice);
                    if (choice != 'y' && choice != 'Y') {
                        return;
                    }
                    continue;
                }
                break;
            } while (1);

            // Price input loop
            do {
                printf("Enter price: ");
                if (scanf("%f", &tempPrice) != 1 || tempPrice < 0) {
                    while (getchar() != '\n');
                    printf("Invalid price. Try again? (y/n): ");
                    scanf(" %c", &choice);
                    if (choice != 'y' && choice != 'Y') {
                        return;
                    }
                    continue;
                }
                break;
            } while (1);

            // Save the item
            strcpy(itemNames[categoryChoice][itemCounts[categoryChoice]], tempName);
            stockLevels[categoryChoice][itemCounts[categoryChoice]] = tempStock;
            reorderPoints[categoryChoice][itemCounts[categoryChoice]] = tempReorder;
            prices[categoryChoice][itemCounts[categoryChoice]] = tempPrice;
            itemCounts[categoryChoice]++;

            // Ask user if they want to view items
            printf("\nItem added successfully! Would you like to view the items? (y/n): ");
            char viewChoice;
            scanf(" %c", &viewChoice);
            if (viewChoice == 'y' || viewChoice == 'Y') {
                displayItems();
            }
        }
        else
        {
            printf("Maximum items for this category reached.\n");
        }
    }
    else
    {
        printf("No categories available. Please add a category first.\n");
    }
}

// Function to manually calculate the length of the string
int my_strlen(char *text)
{
    int len = 0;
    while (text[len] != '\0')
    {
        len++;
    }
    return len;
}

// Function to center-align text based on the total width
void printCentered(char *text, int width)
{
    int len = my_strlen(text); // Using custom strlen function
    int padding = (width - len) / 2;
    if (padding < 0)
        padding = 0; // Ensure we don't have negative padding
    printf("%*s%s%*s", padding, "", text, padding, "");
}

// Function to display categories
void displayCategories()
{
    if (categoryCount == 0) {
        printf("\n┌────────────────────────────────┐\n");
        printf("│    No categories available     │\n");
        printf("└────────────────────────────────┘\n");
        return;
    }

    printf("\n┌─────────────────┬────────────────────────────────────┐\n");
    printf("│   Category ID   │           Category Name            │\n");
    printf("├─────────────────┼────────────────────────────────────┤\n");

    for (int i = 0; i < categoryCount; i++) {
        printf("│      %-10d │", i + 1);
        // Calculate padding for center alignment
        int nameWidth = 34; // Width of category name column
        int nameLength = strlen(categoryNames[i]);
        int leftPadding = (nameWidth - nameLength) / 2;
        int rightPadding = nameWidth - nameLength - leftPadding;
        printf(" %*s%s%*s │\n", leftPadding, "", categoryNames[i], rightPadding, "");
    }

    printf("└─────────────────┴────────────────────────────────────┘\n");
}

// Function to display all items with their details and the category they belong to
void displayItems()
{
    if (categoryCount == 0) {
        printf("\n┌────────────────────────────────┐\n");
        printf("│  No items available to display  │\n");
        printf("└────────────────────────────────┘\n");
        return;
    }

    int choice;
    char retry;
    
    do {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║            VIEW ITEMS MENU             ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║                                        ║\n");
        printf("║   [1] View All Categories              ║\n");
        printf("║   [2] View Specific Category           ║\n");
        printf("║                                        ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("\nEnter choice: ");

        if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
            while (getchar() != '\n');
            printf("\n❌ Invalid choice. Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }
        break;
    } while (1);

    if (choice == 2) {
        // Display categories for selection
        printf("\nChoose a category to view:\n");
        for (int i = 0; i < categoryCount; i++) {
            printf("%d. %s\n", i + 1, categoryNames[i]);
        }

        int categoryChoice;
        do {
            printf("\nEnter category number: ");
            if (scanf("%d", &categoryChoice) != 1 || categoryChoice < 1 || categoryChoice > categoryCount) {
                while (getchar() != '\n');
                printf("\nInvalid category. Try again? (y/n): ");
                scanf(" %c", &retry);
                if (retry != 'y' && retry != 'Y') {
                    return;
                }
                continue;
            }
            break;
        } while (1);

        categoryChoice--; // Adjust for 0-based indexing

        // Display items for selected category
        printf("\n┌─────────────────┬──────────────────┬───────────────┬──────────────┬───────────────┐\n");
        printf("│ Category Name   │    Item Name     │ Stock Level   │ Reorder Point│    Price      │\n");
        printf("├─────────────────┼──────────────────┼───────────────┼──────────────┼───────────────┤\n");

        for (int j = 0; j < itemCounts[categoryChoice]; j++) {
            printf("│ %-16s│ %-17s│ %13d │ %12d │ %13.2f │\n",
                   categoryNames[categoryChoice], itemNames[categoryChoice][j], 
                   stockLevels[categoryChoice][j], reorderPoints[categoryChoice][j], 
                   prices[categoryChoice][j]);
        }

        printf("└─────────────────┴──────────────────┴───────────────┴──────────────┴───────────────┘\n");
    } else {
        // Display all items (original functionality)
        printf("\n┌─────────────────┬──────────────────┬───────────────┬──────────────┬───────────────┐\n");
        printf("│ Category Name   │    Item Name     │ Stock Level   │ Reorder Point│    Price      │\n");
        printf("├─────────────────┼──────────────────┼───────────────┼──────────────┼───────────────┤\n");

        for (int i = 0; i < categoryCount; i++) {
            for (int j = 0; j < itemCounts[i]; j++) {
                printf("│ %-16s│ %-17s│ %13d │ %12d │ %13.2f │\n",
                       categoryNames[i], itemNames[i][j], 
                       stockLevels[i][j], reorderPoints[i][j], prices[i][j]);
            }
        }

        printf("└─────────────────┴──────────────────┴───────────────┴──────────────┴───────────────┘\n");
    }
}

// Function to adjust stock levels
void adjustStock()
{
    if (categoryCount == 0)
    {
        printf("\nNo categories available to adjust stock.\n");
        return;
    }

    // Prompt user to choose add or deduct stock
    int choice;
    char retry;
    
    do {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║          STOCK ADJUSTMENT MENU         ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║                                        ║\n");
        printf("║   [1] ⬆️  Add Stock                     ║\n");
        printf("║   [2] ⬇️  Deduct Stock                  ║\n");
        printf("║                                        ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("\nEnter choice: ");

        if (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2))
        {
            while (getchar() != '\n');
            printf("\n❌ Invalid choice. Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }
        break;
    } while (1);

    // Display categories and items
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      AVAILABLE ITEMS AND STOCKS        ║\n");
    printf("╚════════════════════════════════════════╝\n");

    for (int i = 0; i < categoryCount; i++)
    {
        printf("\n📦 Category: %s\n", categoryNames[i]);
        printf("   ─────────────────────────────────────\n");
        for (int j = 0; j < itemCounts[i]; j++)
        {
            printf("   %d. %-20s [Stock: %d]\n", 
                   (i * MAX_ITEMS_PER_CATEGORY) + j + 1, 
                   itemNames[i][j], 
                   stockLevels[i][j]);
        }
    }

    int itemIndex;
    do {
        printf("\nEnter the item number to adjust stock: ");
        if (scanf("%d", &itemIndex) != 1 || itemIndex < 1 || itemIndex > (categoryCount * MAX_ITEMS_PER_CATEGORY))
        {
            while (getchar() != '\n');
            printf("\nInvalid item selection. Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }
        break;
    } while (1);

    int categoryIndex = (itemIndex - 1) / MAX_ITEMS_PER_CATEGORY;
    int itemInCategoryIndex = (itemIndex - 1) % MAX_ITEMS_PER_CATEGORY;

    // Prompt for date input
    do {
        printf("Enter the date (YYYY-MM-DD): ");
        char date[20];
        if (scanf(" %19s", date) != 1) {
            while (getchar() != '\n');
            printf("Invalid date format. Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }

        // Basic date format validation
        if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') {
            printf("Invalid date format. Please use YYYY-MM-DD format. Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }
        break;
    } while (1);

    int adjustment;
    do {
        printf("Enter the amount to %s: ", (choice == 1) ? "add" : "deduct");
        if (scanf("%d", &adjustment) != 1 || adjustment <= 0) {
            while (getchar() != '\n');
            printf("\nInvalid adjustment amount. Please enter a positive number. Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }

        // For deduction, check if there's enough stock
        if (choice == 2 && adjustment > stockLevels[categoryIndex][itemInCategoryIndex]) {
            printf("\nError: Not enough stock available. Current stock: %d\n", 
                   stockLevels[categoryIndex][itemInCategoryIndex]);
            printf("Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }
        break;
    } while (1);

    // Perform stock adjustment
    if (choice == 1)
    {
        stockLevels[categoryIndex][itemInCategoryIndex] += adjustment;
    }
    else
    {
        stockLevels[categoryIndex][itemInCategoryIndex] -= adjustment;
        if (stockLevels[categoryIndex][itemInCategoryIndex] < 0)
        {
            stockLevels[categoryIndex][itemInCategoryIndex] = 0; // Prevent negative stock levels
        }
    }

    // Update the success message with better formatting
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║          STOCK UPDATE SUCCESS          ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║ Item: %-30s   ║\n", itemNames[categoryIndex][itemInCategoryIndex]);
    printf("║ Category: %-26s   ║\n", categoryNames[categoryIndex]);
    printf("║ New Stock Level: %-20d  ║\n", stockLevels[categoryIndex][itemInCategoryIndex]);
    printf("╚════════════════════════════════════════╝\n");

    // Update the reorder warning with better formatting
    if (stockLevels[categoryIndex][itemInCategoryIndex] < reorderPoints[categoryIndex][itemInCategoryIndex]) {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║        ⚠️  REORDER WARNING  ⚠️           ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║ Stock is below reorder point!          ║\n");
        printf("║ Current Stock: %-22d  ║\n", stockLevels[categoryIndex][itemInCategoryIndex]);
        printf("║ Reorder Point: %-22d  ║\n", reorderPoints[categoryIndex][itemInCategoryIndex]);
        printf("║                                        ║\n");
        printf("║ Please restock soon!                   ║\n");
        printf("╚════════════════════════════════════════╝\n");
    }
}

// Function to search for items
void searchItems()
{
    if (categoryCount == 0) {
        printf("\n┌────────────────────────────────┐\n");
        printf("│  No items available to search  │\n");
        printf("└────────────────────────────────┘\n");
        return;
    }

    int choice;
    char retry;
    
    do {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║            SEARCH MENU                 ║\n");
        printf("╠════════════════════════════════════════╣\n");
        printf("║                                        ║\n");
        printf("║   [1] Search by Item Name              ║\n");
        printf("║   [2] Search by Price Range            ║\n");
        printf("║   [3] Search Low Stock Items           ║\n");
        printf("║                                        ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("\nEnter choice: ");

        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
            while (getchar() != '\n');
            printf("\n❌ Invalid choice. Try again? (y/n): ");
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y') {
                return;
            }
            continue;
        }
        break;
    } while (1);

    switch (choice) {
        case 1: {
            char searchName[MAX_NAME_LENGTH];
            printf("\nEnter item name to search: ");
            while (getchar() != '\n');
            scanf("%[^\n]", searchName);

            printf("\n┌─────────────────┬──────────────────┬───────────────┬──────────────┬───────────────┐\n");
            printf("│ Category Name   │    Item Name     │ Stock Level   │ Reorder Point│    Price      │\n");
            printf("├─────────────────┼──────────────────┼───────────────┼──────────────┼───────────────┤\n");

            int found = 0;
            for (int i = 0; i < categoryCount; i++) {
                for (int j = 0; j < itemCounts[i]; j++) {
                    if (strstr(itemNames[i][j], searchName) != NULL) {
                        printf("│ %-16s│ %-17s│ %13d │ %12d │ %13.2f │\n",
                               categoryNames[i], itemNames[i][j], 
                               stockLevels[i][j], reorderPoints[i][j], 
                               prices[i][j]);
                        found = 1;
                    }
                }
            }
            
            printf("└─────────────────┴──────────────────┴───────────────┴──────────────┴───────────────┘\n");
            if (!found) {
                printf("\nNo items found matching '%s'\n", searchName);
            }

            // Add return to menu prompt
            char returnChoice;
            printf("\nReturn to main menu? (y/n): ");
            scanf(" %c", &returnChoice);
            if (returnChoice != 'y' && returnChoice != 'Y') {
                searchItems(); // Recursively call search function if user wants to search again
            }
            break;
        }
        
        case 2: {
            float minPrice, maxPrice;
            printf("\nEnter minimum price: ");
            scanf("%f", &minPrice);
            printf("Enter maximum price: ");
            scanf("%f", &maxPrice);

            printf("\n┌─────────────────┬──────────────────┬───────────────┬──────────────┬───────────────┐\n");
            printf("│ Category Name   │    Item Name     │ Stock Level   │ Reorder Point│    Price      │\n");
            printf("├─────────────────┼──────────────────┼───────────────┼──────────────┼───────────────┤\n");

            int found = 0;
            for (int i = 0; i < categoryCount; i++) {
                for (int j = 0; j < itemCounts[i]; j++) {
                    if (prices[i][j] >= minPrice && prices[i][j] <= maxPrice) {
                        printf("│ %-16s│ %-17s│ %13d │ %12d │ %13.2f │\n",
                               categoryNames[i], itemNames[i][j], 
                               stockLevels[i][j], reorderPoints[i][j], 
                               prices[i][j]);
                        found = 1;
                    }
                }
            }
            
            printf("└─────────────────┴──────────────────┴───────────────┴──────────────┴───────────────┘\n");
            if (!found) {
                printf("\nNo items found in the price range %.2f - %.2f\n", minPrice, maxPrice);
            }

            // Add return to menu prompt
            char returnChoice;
            printf("\nReturn to main menu? (y/n): ");
            scanf(" %c", &returnChoice);
            if (returnChoice != 'y' && returnChoice != 'Y') {
                searchItems(); // Recursively call search function if user wants to search again
            }
            break;
        }
        
        case 3: {
            printf("\n┌─────────────────┬──────────────────┬───────────────┬──────────────┬───────────────┐\n");
            printf("│ Category Name   │    Item Name     │ Stock Level   │ Reorder Point│    Price      │\n");
            printf("├─────────────────┼──────────────────┼───────────────┼──────────────┼───────────────┤\n");

            int found = 0;
            for (int i = 0; i < categoryCount; i++) {
                for (int j = 0; j < itemCounts[i]; j++) {
                    if (stockLevels[i][j] <= reorderPoints[i][j]) {
                        printf("│ %-16s│ %-17s│ %13d │ %12d │ %13.2f │\n",
                               categoryNames[i], itemNames[i][j], 
                               stockLevels[i][j], reorderPoints[i][j], 
                               prices[i][j]);
                        found = 1;
                    }
                }
            }
            
            printf("└─────────────────┴──────────────────┴───────────────┴──────────────┴───────────────┘\n");
            if (!found) {
                printf("\nNo items found with low stock levels\n");
            }

            // Add return to menu prompt
            char returnChoice;
            printf("\nReturn to main menu? (y/n): ");
            scanf(" %c", &returnChoice);
            if (returnChoice != 'y' && returnChoice != 'Y') {
                searchItems(); // Recursively call search function if user wants to search again
            }
            break;
        }
    }
}