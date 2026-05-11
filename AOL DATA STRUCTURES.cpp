#include <stdio.h> // included for standard input output function
#include <stdlib.h> // included for malloc and free function
#include <string.h> // included for string manipulations function 

// Struct for Trie Node
struct TrieNode {
    struct TrieNode* children[26]; // Array to store child nodes for each alphabet letter
    int isEndOfWord; // Flag to indicate end of a word
    char description[200]; // Description of the slang word
};

// Function to create a new Trie Node
struct TrieNode* createNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode)); // Allocate memory for node
    node->isEndOfWord = 0; // Initialize node as not end of a word
    for (int i = 0; i < 26; i++) { // Initialize all children to NULL
        node->children[i] = NULL;
    }
    return node; // Return the newly created node
}

// Function to validate a slang word (length > 1, no spaces or newline)
int isValidWord(char* word) {
    if (strlen(word) <= 1) return 0; // Must be more than 1 character
    for (int i = 0; word[i]; i++) { // Loop through the letters to validate slang word
        if (word[i] == ' ' || word[i] == '\n') return 0; // Reject if space or newline found
    }
    return 1; // Valid slang word 
}

// Function to validate description (must be more than 1 word)
int isValidDescription(char* description) {
    int wordCount = 0, inWord = 0; // Initialize word count and in word
    for (int i = 0; description[i]; i++) { // Loop through the letters to validate slang word description
        if (description[i] != ' ' && !inWord) {
            wordCount++; // Increment count when entering a word 
            inWord = 1; // Mark that we're inside a word
        } else if (description[i] == ' ') {
            inWord = 0;  // Reset when encountering space
        }
    }
    return wordCount > 1; // Return true if more than 1 word
}

// Function to insert a new slang word into the Trie
void insert(struct TrieNode* root, char* word, char* description) {
    struct TrieNode* current = root; // Start inserting from root
    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a'; // Calculate index based on character 
        if (current->children[index] == NULL) {
            current->children[index] = createNode();  // Create new node if not exist
        }
        current = current->children[index]; // Move to the next node
    }
    current->isEndOfWord = 1; // Mark end of word 
    strcpy(current->description, description); // Store the description
}

// Function to search for a word in the Trie
struct TrieNode* searchNode(struct TrieNode* root, char* word) {
    struct TrieNode* current = root; // Start searching from root
    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a'; // Calculate index based on character
        if (index < 0 || index > 25 || current->children[index] == NULL) {
            return NULL; // Return NULL if not found the word
        }
        current = current->children[index]; // Move to the next node
    }
    return current; // Return the final node
}

// Function to print the slang word and its description
void search(struct TrieNode* root, char* word) {
    struct TrieNode* result = searchNode(root, word); // Get the result by searching the node
    if (result != NULL && result->isEndOfWord) { // Check if it's a complete word
        printf("\nSlang word  : %s\n", word);
        printf("Description : %s\n\n", result->description);
    } else {
        printf("\nThere is no word \"%s\" in the dictionary.\n", word);
    }
}

// Recursive function to collect words from the Trie
void collectWords(struct TrieNode* node, char* prefix, int prefixLen, char** words, int* count) {
    if (node == NULL) return; // If node is NULL, stop 

    if (node->isEndOfWord) {
        words[*count] = (char*)malloc(prefixLen + 1); // Allocate memory for word
        strncpy(words[*count], prefix, prefixLen); // Copy prefix as word
        words[*count][prefixLen] = '\0'; // Null terminate the string
        (*count)++;  // Increase the count of collected words
    }

    for (int i = 0; i < 26; i++) {
        if (node->children[i] != NULL) {
            prefix[prefixLen] = 'a' + i; // Append character 
            collectWords(node->children[i], prefix, prefixLen + 1, words, count); // Recursively collect the words
            prefix[prefixLen] = '\0'; // Reset character after recursion
        }
    }
}

// Function to display words starting with a given prefix
void displayPrefix(struct TrieNode* root, char* prefix) {
    struct TrieNode* current = root; // Start displaying from root
    int prefixLen = strlen(prefix); // Assign the value of prefix length

    for (int i = 0; i < prefixLen; i++) {
        int index = prefix[i] - 'a'; // Get index of current character
        if (current->children[index] == NULL) {
            printf("\nThere is no prefix \"%s\" in the dictionary.\n", prefix);
            return;
        }
        current = current->children[index]; // Move to the next node
    }

    char temp[50]; // Temporary buffer to hold prefix and build words
    strcpy(temp, prefix); // Copy prefix to temporary buffer 
    char* words[100]; // Array to store collected words 
    int count = 0; // Counter for number of words collected
    collectWords(current, temp, prefixLen, words, &count); // Collect all matching words

    if (count == 0) { // If no words collected 
        printf("There is no prefix \"%s\" in the dictionary.\n", prefix);   
    } else {
        for (int i = 0; i < count - 1; i++) { // Sort the words lexicographically using bubble sort
            for (int j = i + 1; j < count; j++) {
                if (strcmp(words[i], words[j]) > 0) {
                    char* temp = words[i];
                    words[i] = words[j];
                    words[j] = temp;
                }
            }
        }

        printf("\nWords starts with \"%s\":\n", prefix);
        for (int i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, words[i]);
            free(words[i]); // Free memory after printing
        } printf("\n");
    }
}

// Function to display all words in the Trie
void displayAllWords(struct TrieNode* root) {
    char* words[100]; // Array to store all collected words 
    int count = 0; // Counter for number of words 
    char temp[50] = ""; // Empty prefix to start from root

    collectWords(root, temp, 0, words, &count); // Collect all words from Trie

    if (count == 0) { // If dictionary is empty
        printf("There is no slang word yet in the dictionary.\n"); 
    } else {
    	// Sort all collected words
        for (int i = 0; i < count - 1; i++) { 
            for (int j = i + 1; j < count; j++) {
                if (strcmp(words[i], words[j]) > 0) {
                    char* temp = words[i];
                    words[i] = words[j];
                    words[j] = temp;
                }
            }
        }

        printf("List of all slang words in the dictionary:\n");
        for (int i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, words[i]);
            free(words[i]); // Free memory after printing
        } printf("\n");
    }
}

// Function to pause and wait for the user to press enter
void pressEnterToContinue() {
    printf("Press enter to continue...");
    while (getchar() != '\n'); // Wait until Enter is pressed
}

// Main function
int main() {
    struct TrieNode* root = createNode(); // Initialize root of trie
    int choice; // To input the number of choice

    do {
    	// Menu
        printf("=== Boogle ===\n");
        printf("1. Release a new slang word\n");
        printf("2. Search a slang word\n");
        printf("3. View all slang words starting with a certain prefix word\n");
        printf("4. View all slang words\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice); getchar(); // Get user input (number) of choice and remove newline from buffer 

        if (choice == 1) {
            char word[50], description[200];  // Array to store words and descriptions 

            while (1) {
            	 // Get slang word input
                printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
                fgets(word, sizeof(word), stdin); /* fgets is used to safely read string with spaces.  fgets instead of scanf: 
				fgets reads the entire line including spaces and prevents buffer overflow, making it safer and more 
				flexible than scanf("%s") which stops at space. */
                word[strcspn(word, "\n")] = '\0'; // Remove trailing newline 
                if (isValidWord(word)) break;  // Break loop if word is valid
            }

            while (1) {
            	 // Get description input
                printf("Input a new slang word description [Must be more than 2 words]: ");
                fgets(description, sizeof(description), stdin); // Using fgets for same reasons
                description[strcspn(description, "\n")] = '\0'; // Remove trailing newline
                if (isValidDescription(description)) break; // Break loop if description is valid
            }
			// Check if the word exists already
            struct TrieNode* node = searchNode(root, word);
            if (node != NULL && node->isEndOfWord) {
                strcpy(node->description, description); // Update existing description
                printf("\nSuccessfully updated a slang word.\n");
            } else {
                insert(root, word, description); // Insert new word and description
                printf("\nSuccessfully released new slang word.\n");
            }

            pressEnterToContinue();
        }

        else if (choice == 2) {
            char word[50]; // Array to store words

            while (1) {
                printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]: ");
                fgets(word, sizeof(word), stdin); // Using fgets for same reasons
                word[strcspn(word, "\n")] = '\0'; // Remove newline
                if (isValidWord(word)) break; // Break loop if word is valid
            }

            search(root, word); // Search the word in dictionary
            pressEnterToContinue();
        }

        else if (choice == 3) {
            char prefix[50]; // Array to store prefix
            printf("Input a prefix to be searched: ");
            fgets(prefix, sizeof(prefix), stdin); // Using fgets for same reasons
            prefix[strcspn(prefix, "\n")] = '\0'; // Remove newline
            displayPrefix(root, prefix); // Show all words with given prefix
            pressEnterToContinue();
        }

        else if (choice == 4) {
            displayAllWords(root); // Show all slang words
            pressEnterToContinue();
        }

        else if (choice == 5) {
            printf("Thank you... Have a nice day :)\n"); // Exit message 
        }

        else {
            printf("Invalid choice, please try again.\n"); // Handle invalid menu choice
            pressEnterToContinue();
        }

    } while (choice != 5); // Loop until user chooses to exit

    return 0;  // End of program
}
