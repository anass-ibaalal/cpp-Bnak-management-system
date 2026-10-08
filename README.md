# Bnak management system (C++)

A bank management system built using C++, This project demonstrates file handling for data presistence, dynamic record processing using vectors, and clean moduler code architecture.

# Features

### 1. authentication & Security
- **User login system**: Secure authentication with username and password verification.
- **Role-Based access control (RBAC)**: Fine-grained permission system implemented using **Bitwise Operations** to restrict access per menu (Show, Add, Delete, Update, Transactions, Manage Users, Full Access).
   
### 2. Client Management (CRUD Operations)
- **View Clients**: Formatted ASCII table display of client records (Account Number, PIN, Name, Phone, Balance).
- **Add New Client**: Create new accounts with automatic duplicate Account Number validation.
- **Find Client**: Search client records instantly by account number.
- **Update Client**: Modify client details safely.
- **Delete Client**: Remove client records with confirmation safety checks.

### 3. Banking Transactions
- **Deposit**: Credit money to any account with automatic file balance updates.
- **Withdrawal**: Debit funds with real-time balance validation (prevents overdrafts).
- **Total Balances**: Aggregate and display total system balance across all accounts.

### 4. User Management (Admin Panel)
- **User CRUD System**: List, add, update, and search system users.
- **Permission Assignment**: Granular access control setup during user creation.
- **Admin Safety Guard**: System protection preventing deletion of the primary `Admin` account.

### 5. Data Persistence
- **File-Based Storage**: Persistent data management using text files (`Clients.txt` and `Users.txt`).
- **Custom Serialization**: Custom string parser using custom delimiters (`/#/`) for data parsing and vector mapping.
