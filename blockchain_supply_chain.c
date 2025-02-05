#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h>
#include <openssl/ec.h>
#include <openssl/objects.h>

#define DIFFICULTY 4
#define MAX_TRANSACTIONS 10

// Cryptographic Functions
char* calculate_hash(char* data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, data, strlen(data));
    SHA256_Final(hash, &sha256_ctx);

    char* hex_hash = malloc(65);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex_hash + (i * 2), "%02x", hash[i]);
    }
    hex_hash[64] = '\0';
    return hex_hash;
}

ECDSA_SIG* sign_data(EC_KEY* key, const char* data) {
    unsigned char* sig = NULL;
    unsigned int sig_len = 0;
    ECDSA_SIG* signature = NULL;

    sig_len = ECDSA_size(key);
    sig = malloc(sig_len);

    if (ECDSA_sign(0, (unsigned char*)data, strlen(data), sig, &sig_len, key)) {
        signature = ECDSA_SIG_new();
        if (signature) {
            ECDSA_SIG_set0(signature, BN_bin2bn(sig, sig_len, NULL), BN_bin2bn(sig + (sig_len / 2), sig_len / 2, NULL));
        }
    }
    free(sig);
    return signature;
}

int verify_signature(EC_KEY* pubkey, const char* data, ECDSA_SIG* sig) {
    return ECDSA_verify(0, (unsigned char*)data, strlen(data), (unsigned char*)sig, ECDSA_size(pubkey), pubkey);
}

// Transaction Struct
typedef struct {
    char item_id[64];
    char item_description[256];
    ECDSA_SIG* signature;
    EC_KEY* public_key;
} Transaction;

// Block Struct
typedef struct {
    int index;
    char previous_hash[65];
    Transaction* transactions[MAX_TRANSACTIONS];
    int transaction_count;
    time_t timestamp;
    int nonce;
    char hash[65];
} Block;

// Blockchain Struct
typedef struct {
    Block* chain[100];
    int block_count;
    Transaction* pending_transactions[MAX_TRANSACTIONS];
    int pending_transaction_count;
    double wallet_balance;
} Blockchain;

// Functions to create and manage the blockchain
Blockchain* create_blockchain() {
    Blockchain* blockchain = malloc(sizeof(Blockchain));
    blockchain->block_count = 0;
    blockchain->pending_transaction_count = 0;
    blockchain->wallet_balance = 0.0;
    return blockchain;
}

Block* create_block(Blockchain* blockchain, int index, char* previous_hash, Transaction* transactions[], int transaction_count) {
    Block* new_block = malloc(sizeof(Block));
    new_block->index = index;
    strcpy(new_block->previous_hash, previous_hash);
    new_block->transaction_count = transaction_count;
    for (int i = 0; i < transaction_count; i++) {
        new_block->transactions[i] = transactions[i];
    }
    new_block->timestamp = time(NULL);
    new_block->nonce = 0;
    return new_block;
}

void add_transaction(Blockchain* blockchain, Transaction* transaction) {
    if (blockchain->pending_transaction_count < MAX_TRANSACTIONS) {
        blockchain->pending_transactions[blockchain->pending_transaction_count++] = transaction;
    } else {
        printf("Error: Max transaction limit reached for this block.\n");
    }
}

void mine_block(Blockchain* blockchain, Block* block) {
    char target[DIFFICULTY + 1];
    memset(target, '0', DIFFICULTY);
    target[DIFFICULTY] = '\0';

    char block_data[1024];
    while (1) {
        sprintf(block_data, "%d%s%d", block->index, block->previous_hash, block->nonce);
        char* block_hash = calculate_hash(block_data);
        if (strncmp(block_hash, target, DIFFICULTY) == 0) {
            strcpy(block->hash, block_hash);
            free(block_hash);
            break;
        }
        free(block_hash);
        block->nonce++;
    }
}

void print_blockchain(Blockchain* blockchain) {
    for (int i = 0; i < blockchain->block_count; i++) {
        Block* block = blockchain->chain[i];
        printf("Block %d: Hash: %s Previous Hash: %s\n", block->index, block->hash, block->previous_hash);
        for (int j = 0; j < block->transaction_count; j++) {
            printf("  Transaction %d: Item ID: %s, Description: %s\n", j, block->transactions[j]->item_id, block->transactions[j]->item_description);
        }
        printf("\n");
    }
}

void load_wallet(Blockchain* blockchain) {
    // Simulate loading the wallet with some initial funds
    blockchain->wallet_balance = 1000.0;
    printf("Wallet loaded successfully! Balance: $%.2f\n", blockchain->wallet_balance);
}

void add_funds(Blockchain* blockchain, double amount) {
    blockchain->wallet_balance += amount;
    printf("Funds added successfully! New Balance: $%.2f\n", blockchain->wallet_balance);
}

void view_balance(Blockchain* blockchain) {
    printf("Current Wallet Balance: $%.2f\n", blockchain->wallet_balance);
}

// CLI Functions
void display_menu() {
    printf("\nBlockchain Wallet Management - Menu:\n");
    printf("1. Load Existing Wallet\n");
    printf("2. Add Funds\n");
    printf("3. View Balance\n");
    printf("4. Exit\n");
}

int main() {
    Blockchain* blockchain = NULL;
    EC_KEY* key = EC_KEY_new_by_curve_name(NID_secp256k1);
    EC_KEY_generate_key(key);

    int choice;
    while (1) {
        display_menu();
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        if (choice == 1) {
            blockchain = create_blockchain();
            load_wallet(blockchain);
        } 
        else if (choice == 2) {
            if (blockchain == NULL) {
                printf("Error: Wallet not loaded yet!\n");
                continue;
            }
            double amount;
            printf("Enter the amount to add: $");
            scanf("%lf", &amount);
            add_funds(blockchain, amount);
        } 
        else if (choice == 3) {
            if (blockchain == NULL) {
                printf("Error: Wallet not loaded yet!\n");
                continue;
            }
            view_balance(blockchain);
        } 
        else if (choice == 4) {
            printf("Exiting the program...\n");
            break;
        } 
        else {
            printf("Invalid choice! Please enter a valid option (1-4).\n");
        }
    }

    return 0;
}
