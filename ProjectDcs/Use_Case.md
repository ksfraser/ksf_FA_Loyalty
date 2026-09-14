# Use Cases - FA Loyalty Module

## Document Information
- **Module**: ksf_FA_Loyalty
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Actor Definitions

### 1.1 Primary Actors

| Actor | Description | Permissions |
|-------|-------------|-------------|
| **Staff User** | FA staff member | SA_LOYALTYVIEW |
| **Admin User** | FA administrator | SA_LOYALTYVIEW, SA_LOYALTYCREATE |
| **Customer** | CRM customer record | Indirect (via staff actions) |

### 1.2 Secondary Actors

| Actor | Description |
|-------|-------------|
| **FA System** | FrontAccounting ERP backend |
| **CRM System** | Customer relationship management |
| **Database** | MySQL database containing loyalty data |

---

## 2. Use Cases

### UC-LOY-001: Enroll Customer in Loyalty Program

**Description**: Admin enrolls a new customer in the loyalty program.

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-001 |
| Actor | Admin User |
| Goal | Add customer to loyalty program |
| Priority | High |

**Pre-conditions**:
- Customer exists in CRM (0_crm_persons table)
- Customer not already enrolled
- User has SA_LOYALTYCREATE permission

**Post-conditions**:
- New loyalty record created
- Customer can earn/redeem points
- Default tier (Bronze) assigned

**Basic Flow**:
```
1. Admin navigates to CRM application
2. Admin clicks "Loyalty Program" menu
3. Admin clicks "Enroll New Customer" button
4. System displays enrollment form
5. Admin searches for and selects customer
6. Admin reviews customer details
7. Admin confirms enrollment
8. System validates customer not already enrolled
9. System creates loyalty record with:
   - loyalty_id (auto-increment)
   - customer_person_id (from selection)
   - points_balance = 0
   - tier_level = 'Bronze'
   - enrollment_date = current date
10. System displays success message
11. Loyalty record visible in program list
```

**Alternative Flows**:

*AF-LOY-001a: Customer Already Enrolled*
```
6a. System detects customer already enrolled
6b. System displays error: "Customer already enrolled in loyalty program"
6c. Admin can view existing loyalty record instead
```

*AF-LOY-001b: Customer Not Found*
```
5a. Search returns no results
5b. System displays "No matching customers found"
5c. Admin can create new customer in CRM first
```

---

### UC-LOY-002: View Customer Loyalty Status

**Description**: Staff/Admin views loyalty information for a customer.

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-002 |
| Actor | Staff User, Admin User |
| Goal | Review customer's loyalty program status |
| Priority | High |

**Pre-conditions**:
- Customer enrolled in loyalty program
- User has SA_LOYALTYVIEW permission

**Post-conditions**:
- Loyalty information displayed to user

**Basic Flow**:
```
1. User navigates to Loyalty Program
2. User searches for customer by name/email
3. System retrieves loyalty record from fa_customer_loyalty
4. System joins with CRM data for customer name/email
5. System displays loyalty card with:
   - Customer Name
   - Email
   - Loyalty ID
   - Points Balance
   - Current Tier
   - Enrollment Date
   - Recent Transactions (last 10)
```

**Exception Flows**:

*EF-LOY-002a: Customer Not Enrolled*
```
4a. No loyalty record found for customer
4b. System displays: "Customer is not enrolled in loyalty program"
4c. Option to enroll customer shown (admin only)
```

---

### UC-LOY-003: Award Points to Customer

**Description**: Admin awards loyalty points to customer (purchase reward, bonus, etc.).

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-003 |
| Actor | Admin User |
| Goal | Add points to customer balance |
| Priority | High |

**Pre-conditions**:
- Customer enrolled in loyalty program
- User has SA_LOYALTYCREATE permission
- Points value is positive integer

**Post-conditions**:
- Points added to customer balance
- Transaction record created
- Transaction history updated

**Basic Flow**:
```
1. Admin navigates to Loyalty Program
2. Admin locates customer
3. Admin clicks "Add Points" or "Award Points"
4. System displays points award form:
   - Points Amount (required, positive)
   - Reference Type (ORDER/INVOICE/ADMIN/MANUAL)
   - Reference ID (optional)
   - Description (optional)
5. Admin enters points amount: 100
6. Admin selects reference type: "ORDER"
7. Admin enters reference ID: "ORD-2024-001"
8. Admin enters description: "Purchase reward"
9. Admin clicks "Award Points"
10. System validates:
    - Points amount > 0
    - Customer enrolled (loyalty_id exists)
11. System executes:
    - UPDATE fa_customer_loyalty SET points_balance = points_balance + 100
    - INSERT INTO fa_loyalty_transactions (...)
12. System displays success: "100 points awarded to customer"
13. Points balance updated in display
```

---

### UC-LOY-004: Redeem Points

**Description**: Admin processes points redemption for customer.

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-004 |
| Actor | Admin User |
| Goal | Deduct points from customer balance |
| Priority | High |

**Pre-conditions**:
- Customer enrolled in loyalty program
- Customer has sufficient points balance
- User has SA_LOYALTYCREATE permission

**Post-conditions**:
- Points deducted from customer balance
- Transaction record created with type 'REDEEM'

**Basic Flow**:
```
1. Admin navigates to Loyalty Program
2. Admin locates customer
3. Admin clicks "Redeem Points"
4. System displays redemption form
5. Admin enters points to redeem: 50
6. Admin selects reason: "Discount"
7. Admin clicks "Redeem"
8. System validates:
    - Points amount > 0
    - Points amount <= customer balance
9. System executes:
    - UPDATE fa_customer_loyalty SET points_balance = points_balance - 50
    - INSERT INTO fa_loyalty_transactions (type='REDEEM', ...)
10. System displays success
```

**Exception Flows**:

*EF-LOY-004a: Insufficient Points*
```
8a. Points amount > customer balance
8b. System displays: "Insufficient points. Available: 30 points"
8c. Admin can adjust redemption amount
```

*EF-LOY-004b: Zero Balance*
```
6a. Customer has 0 points
6b. System displays: "No points available for redemption"
6c. Admin can explain loyalty benefits to customer
```

---

### UC-LOY-005: Adjust Points (Admin Correction)

**Description**: Admin manually adjusts points balance to correct errors.

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-005 |
| Actor | Admin User |
| Goal | Correct loyalty points balance |
| Priority | Medium |

**Pre-conditions**:
- Customer enrolled in loyalty program
- User has SA_LOYALTYCREATE permission

**Post-conditions**:
- Points balance adjusted by specified amount
- Transaction created with type 'ADJUST'
- Audit trail includes reason for adjustment

**Basic Flow**:
```
1. Admin navigates to Loyalty Program
2. Admin locates customer
3. Admin clicks "Adjust Points"
4. System displays adjustment form
5. Admin enters:
   - Adjustment Type: "Add" or "Deduct"
   - Points Amount: 25
   - Reason: "Correction for missing bonus points"
6. Admin clicks "Submit Adjustment"
7. System validates reason is provided
8. System executes adjustment and creates ADJUST transaction
9. System displays success with new balance
```

---

### UC-LOY-006: View Transaction History

**Description**: User views complete transaction history for loyalty customer.

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-006 |
| Actor | Staff User, Admin User |
| Goal | Review loyalty activity |
| Priority | High |

**Pre-conditions**:
- Customer enrolled in loyalty program
- User has SA_LOYALTYVIEW permission

**Post-conditions**:
- Transaction history displayed

**Basic Flow**:
```
1. User views customer loyalty status (UC-LOY-002)
2. User clicks "View Full History" or "Transactions" tab
3. System retrieves transactions from fa_loyalty_transactions
4. System orders by created_at DESC
5. System displays transaction list:
   - Date
   - Type (EARN/REDEEM/ADJUST/EXPIRE)
   - Points (+/-)
   - Description
   - Reference
6. Pagination if > 50 transactions
```

---

### UC-LOY-007: Update Tier Level

**Description**: Admin changes customer's loyalty tier.

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-007 |
| Actor | Admin User |
| Goal | Upgrade or downgrade customer tier |
| Priority | Medium |

**Pre-conditions**:
- Customer enrolled in loyalty program
- User has SA_LOYALTYCREATE permission

**Post-conditions**:
- Tier level updated in database

**Basic Flow**:
```
1. Admin navigates to customer loyalty detail
2. Admin clicks "Change Tier"
3. System displays tier selection:
   - Bronze
   - Silver
   - Gold
   - Platinum
4. Admin selects new tier: "Silver"
5. Admin clicks "Update Tier"
6. System updates tier_level in fa_customer_loyalty
7. System displays success with new tier
```

---

### UC-LOY-008: Remove Customer from Loyalty Program

**Description**: Admin removes customer from loyalty program.

| Attribute | Value |
|-----------|-------|
| ID | UC-LOY-008 |
| Actor | Admin User |
| Goal | Delete customer from loyalty system |
| Priority | Low |

**Pre-conditions**:
- Customer enrolled in loyalty program
- User has SA_LOYALTYCREATE permission

**Post-conditions**:
- Loyalty record deleted
- Transaction history may be preserved or deleted

**Basic Flow**:
```
1. Admin navigates to customer loyalty detail
2. Admin clicks "Remove from Program"
3. System displays confirmation dialog
4. Admin confirms removal
5. System deletes fa_customer_loyalty record
6. Transaction history preserved (for audit)
7. System displays success message
```

---

## 3. Use Case Diagram

```
                    ┌─────────────────────┐
                    │    Staff User      │
                    │  (SA_LOYALTYVIEW)   │
                    └─────────────────────┘
                                │
           ┌────────────────────┴────────────────────┐
           │                                         │
           ▼                                         ▼
    ┌─────────────────┐                      ┌─────────────────┐
    │ View Loyalty    │                      │ View History    │
    │  Status         │                      │                 │
    │ (UC-LOY-002)    │                      │ (UC-LOY-006)    │
    └─────────────────┘                      └─────────────────┘

                    ┌─────────────────────┐
                    │    Admin User       │
                    │ (SA_LOYALTYCREATE)  │
                    └─────────────────────┘
                                │
     ┌──────────────────────────┼──────────────────────────┐
     │                          │                          │
     ▼                          ▼                          ▼
┌─────────────┐         ┌─────────────┐           ┌─────────────┐
│   Enroll    │         │   Award    │           │   Redeem    │
│  Customer   │         │   Points   │           │   Points    │
│ (UC-LOY-001)│         │ (UC-LOY-003)│           │ (UC-LOY-004)│
└─────────────┘         └─────────────┘           └─────────────┘
                               │                          │
                               │                          │
                               ▼                          ▼
                        ┌─────────────┐           ┌─────────────┐
                        │  Adjust     │           │ Update      │
                        │  Points     │           │ Tier        │
                        │ (UC-LOY-005)│           │(UC-LOY-007) │
                        └─────────────┘           └─────────────┘
                                                         │
                                                         ▼
                                                  ┌─────────────┐
                                                  │   Remove    │
                                                  │   from      │
                                                  │   Program   │
                                                  │ (UC-LOY-008)│
                                                  └─────────────┘
```

---

## 4. Business Rules

| Rule ID | Description |
|---------|-------------|
| BR-LOY-001 | Customer can only be enrolled once (UNIQUE constraint) |
| BR-LOY-002 | New enrollments default to Bronze tier |
| BR-LOY-003 | Points balance cannot be negative (application validation) |
| BR-LOY-004 | All point changes require transaction record |
| BR-LOY-005 | Transaction history preserved even if customer removed |
| BR-LOY-006 | Only enrolled customers can earn/redeem points |
| BR-LOY-007 | Only users with SA_LOYALTYCREATE can modify points |
| BR-LOY-008 | All users with SA_LOYALTYVIEW can view loyalty info |

---

## 5. Requirements to Use Case Mapping

| Requirement | Use Case(s) |
|-------------|-------------|
| FR-LOY-001 | UC-LOY-001 |
| FR-LOY-002 | UC-LOY-002 |
| FR-LOY-003 | UC-LOY-002 (via lookup) |
| FR-LOY-010 | UC-LOY-003 |
| FR-LOY-011 | UC-LOY-004 |
| FR-LOY-012 | UC-LOY-002 |
| FR-LOY-020 | UC-LOY-003, UC-LOY-004, UC-LOY-005 |
| FR-LOY-021 | UC-LOY-006 |
| FR-LOY-030 | UC-LOY-001 (default) |
| FR-LOY-031 | UC-LOY-007 |
| FR-LOY-040 | CRM menu access |
| FR-LOY-041 | Sales menu access |

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*