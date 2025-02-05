 Blockchain Wallet Management

This is a simple C program that simulates blockchain wallet management, allowing users to load a wallet, add funds, and view their balance. It uses cryptographic functions to demonstrate basic blockchain concepts such as hashing and digital signatures.

Features

- Load Existing Wallet: Simulates loading a wallet and initializes the balance.
- Add Funds: Allows the user to add funds to their wallet.
- View Balance: Displays the current balance of the wallet.
- Exit: Exits the program.

Requirements

- C Compiler
- OpenSSL library for cryptographic functions (SHA256, ECDSA, etc.)

Installation

1. Ensure that you have a C compiler installed (e.g., GCC).
2. Install OpenSSL development libraries.

   On Linux (Ubuntu):
   
   sudo apt-get install libssl-dev
   
3. Clone this repository:
  
   git clone https://github.com/yourusername/blockchain_supply_chain.git
   cd blockchain-wallet
  

4. Compile the program:
  
   gcc blockchain_supply_chain.c -o blockchain_supply_chain -lssl -lcrypto
   

Usage

1. Run the compiled program:
   ./blockchain_supply_chain
2. Use the interactive menu:
   - 1: Load Existing Wallet
   - 2: Add Funds
   - 3: View Balance
   - 4: Exit

Example Interaction

Blockchain Wallet Management - Menu:
1. Load Existing Wallet
2. Add Funds
3. View Balance
4. Exit
Enter your choice (1-4): 1
Wallet loaded successfully! Balance: $1000.00

Blockchain Wallet Management - Menu:
1. Load Existing Wallet
2. Add Funds
3. View Balance
4. Exit
Enter your choice (1-4): 2
Enter the amount to add: $500.00
Funds added successfully! New Balance: $1500.00

Blockchain Wallet Management - Menu:
1. Load Existing Wallet
2. Add Funds
3. View Balance
4. Exit
Enter your choice (1-4): 3
Current Wallet Balance: $1500.00

License
