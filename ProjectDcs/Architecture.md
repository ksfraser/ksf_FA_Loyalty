# Architecture - FA Loyalty Module

## Document Information
- **Module**: ksf_FA_Loyalty
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-24

---

## 1. Technical Architecture

### 1.1 Architecture Pattern
The FA Loyalty module follows the **Business Logic + Platform Adapter** pattern:

```
┌─────────────────────────────────────────────────────────┐
│                   FrontAccounting UI                     │
├─────────────────────────────────────────────────────────┤
│               loyalty.php (Presentation)                 │
├─────────────────────────────────────────────────────────┤
│           includes/loyalty_db.inc                        │
│           includes/loyalty_transactions_db.inc          │
│           includes/coupons_db.inc                        │
├─────────────────────────────────────────────────────────┤
│                  hooks.php (Integration)                 │
├─────────────────────────────────────────────────────────┤
│                 ksf_FA_CRM (Core)                       │
└─────────────────────────────────────────────────────────┘
```

### 1.2 Module Components

| Component | File | Responsibility |
|-----------|------|----------------|
| Hooks | hooks.php | FA integration, menu registration, table creation |
| Customer Loyalty DB | includes/loyalty_db.inc | CRUD for fa_customer_loyalty |
| Transaction DB | includes/loyalty_transactions_db.inc | CRUD for fa_loyalty_transactions |
| Coupon DB | includes/coupons_db.inc | Coupon management (future) |
| SQL | sql/*.sql | Database schema definition |

---

## 2. Class Diagram

### 2.1 Hooks Class Structure

```
┌─────────────────────────────────────────┐
│              hooks_fa_loyalty           │
├─────────────────────────────────────────┤
│ - module_name: string = 'fa_loyalty'    │
├─────────────────────────────────────────┤
│ + install_options($app): void          │
│ + install_access(): array               │
│ + activate_extension($company, $check_only): bool │
│ + db_prevoid($trans_type, $trans_no): void│
└─────────────────────────────────────────┘
         │
         │ extends
         ▼
┌─────────────────────────────────────────┐
│                 hooks                   │
├─────────────────────────────────────────┤
│ (FrontAccounting base hooks class)      │
└─────────────────────────────────────────┘
```

### 2.2 Database Schema

```
┌──────────────────────────────────────────────────────────┐
│                  fa_customer_loyalty                     │
├──────────────────────────────────────────────────────────┤
│ PK  loyalty_id         INT(11) AUTO_INCREMENT            │
│ FK  customer_person_id INT(11) NOT NULL                  │
│     points_balance     INT(11) DEFAULT 0                 │
│     tier_level         VARCHAR(20) DEFAULT 'Bronze'      │
│     enrollment_date    DATE DEFAULT NULL                  │
│     created_at         TIMESTAMP                          │
├──────────────────────────────────────────────────────────┤
│     │                                                  │
│     │ 1:N                                               │
│     │                                                  │
│     ▼                                                  │
├──────────────────────────────────────────────────────────┐
│                  fa_loyalty_transactions                 │
├──────────────────────────────────────────────────────────┤
│ PK  transaction_id    INT(11) AUTO_INCREMENT              │
│ FK  loyalty_id        INT(11) NOT NULL                   │
│     transaction_type  VARCHAR(20) NOT NULL               │
│     points            INT(11) NOT NULL                    │
│     reference_type    VARCHAR(20)                        │
│     reference_id      VARCHAR(100)                       │
│     description       TEXT                                │
│     created_at        TIMESTAMP                           │
└──────────────────────────────────────────────────────────┘
          │
          │ References
          ▼
┌──────────────────────────────────────────────────────────┐
│                    0_crm_persons                         │
├──────────────────────────────────────────────────────────┤
│ PK  id                INT                               │
│     name              VARCHAR(100)                      │
│     email             VARCHAR(100)                      │
│     ...other CRM fields...                               │
└──────────────────────────────────────────────────────────┘
```

---

## 3. Data Flow

### 3.1 Customer Loyalty Enrollment Flow

```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│   CRM User  │      │  loyalty.php│      │  loyalty_db │
│   Request   │─────▶│  (Form UI)  │─────▶│   .inc      │
│             │      │             │      │  (CRUD)     │
└─────────────┘      └─────────────┘      └─────────────┘
                                                  │
                                                  ▼
                                           ┌─────────────┐
                                           │   Database   │
                                           │fa_customer   │
                                           │_loyalty      │
                                           └─────────────┘
```

### 3.2 Points Transaction Flow

```
┌─────────────┐      ┌─────────────┐      ┌─────────────────┐
│   Sales     │      │  loyalty    │      │loyalty_transa  │
│   Order     │─────▶│_transactions │─────▶│ctions_db.inc   │
│   Complete  │      │    .inc      │      │                 │
└─────────────┘      └─────────────┘      └─────────────────┘
                                                  │
                                                  ▼
                                           ┌─────────────┐
                                           │   Database   │
                                           │fa_loyalty_   │
                                           │transactions  │
                                           └─────────────┘
                                                  │
                                                  ▼
                                           ┌─────────────┐
                                           │Update points │
                                           │balance       │
                                           └─────────────┘
```

### 3.3 Module Activation Flow

```
┌──────────────────────────────────────────────────────────┐
│                   FA Module Manager                      │
│                   (Extension Activation)                 │
└──────────────────────────────────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────┐
│           hooks_fa_loyalty::activate_extension()          │
└──────────────────────────────────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────┐
│    Execute SQL files:                                    │
│    - sql/fa_customer_loyalty.sql                         │
│    - sql/fa_loyalty_transactions.sql                     │
└──────────────────────────────────────────────────────────┘
```

---

## 4. Security Architecture

### 4.1 Security Sections and Areas

| Section Constant | Value | Description |
|-----------------|-------|-------------|
| SS_LOYALTY | 118 << 8 (30208) | Loyalty Program Section |

| Area Constant | Value | Description |
|---------------|-------|-------------|
| SA_LOYALTYVIEW | SS_LOYALTY \| 1 | View Loyalty Information |
| SA_LOYALTYCREATE | SS_LOYALTY \| 2 | Manage Loyalty Program |

### 4.2 Access Control Matrix

| Function | SA_LOYALTYVIEW | SA_LOYALTYCREATE |
|----------|-----------------|-------------------|
| View loyalty status | ✓ | ✓ |
| View points balance | ✓ | ✓ |
| View transaction history | ✓ | ✓ |
| Enroll new customer | ✗ | ✓ |
| Adjust points | ✗ | ✓ |
| Change tier level | ✗ | ✓ |
| Create/void transactions | ✗ | ✓ |

---

## 5. Menu Structure

### 5.1 CRM Application Menu

```
CRM Application
└── Right Panel Functions
    └── Loyalty Program          [SA_LOYALTYVIEW]
```

### 5.2 Sales Application Menu

```
Sales Application
└── Left Panel Functions
    └── Customer Loyalty         [SA_LOYALTYVIEW]
```

---

## 6. Function Reference

### 6.1 loyalty_db.inc Functions

| Function | Purpose | Parameters | Return |
|----------|---------|------------|--------|
| write_customer_loyalty() | Create/Update loyalty record | loyalty_id, data[] | loyalty_id |
| get_customer_loyalty() | Get loyalty by ID | loyalty_id | array |
| get_loyalty_by_customer() | Get loyalty by customer | person_id | array |
| update_loyalty_points() | Add/subtract points | loyalty_id, points_change | void |
| delete_customer_loyalty() | Remove loyalty record | loyalty_id | void |

### 6.2 loyalty_transactions_db.inc Functions

| Function | Purpose | Parameters | Return |
|----------|---------|------------|--------|
| write_loyalty_transaction() | Create/Update transaction | transaction_id, data[] | transaction_id |
| get_loyalty_transactions() | Get transactions for loyalty | loyalty_id, limit | result |
| get_transaction() | Get single transaction | transaction_id | array |
| delete_loyalty_transaction() | Remove transaction | transaction_id | void |

---

## 7. Database Indexes

| Table | Index | Type | Purpose |
|-------|-------|------|---------|
| fa_customer_loyalty | idx_customer (UNIQUE) | KEY | Customer lookup |

---

## 8. Integration Points

### 8.1 FrontAccounting Hooks

| Hook Name | Purpose | Implementation |
|-----------|---------|----------------|
| install_options | Menu registration | CRM and Sales app functions |
| install_access | Security setup | Security area registration |
| activate_extension | Module activation | SQL file execution |
| db_prevoid | Transaction voiding | Placeholder (future) |

### 8.2 CRM Integration

| Point | Description |
|-------|-------------|
| CRM Persons Table | Foreign key relationship to 0_crm_persons |
| Customer Lookup | Link loyalty to CRM customer records |
| Customer Display | Show loyalty status in CRM views |

---

## 9. File Structure

```
ksf_FA_Loyalty/
├── hooks.php                    # FA hooks and integration
├── loyalty.php                  # Loyalty UI page
├── includes/
│   ├── loyalty_db.inc           # Customer loyalty CRUD
│   ├── loyalty_transactions_db.inc # Transaction CRUD
│   └── coupons_db.inc          # Coupon management (future)
├── sql/
│   ├── fa_customer_loyalty.sql  # Customer loyalty schema
│   └── fa_loyalty_transactions.sql # Transaction schema
├── composer.json
└── ProjectDcs/
    ├── Business_Requirements.md
    ├── Architecture.md           # This file
    ├── Functional_Requirements.md
    ├── Use_Case.md
    ├── Test_Plan.md
    └── UAT_Plan.md
```

---

## 10. Error Handling

### 10.1 Database Errors
- All database operations use db_query() with error logging
- Foreign key constraint violations caught with descriptive messages
- Duplicate customer enrollment prevented with UNIQUE constraint

### 10.2 Validation Errors
- Required field validation before submission
- Points balance non-negative check
- Valid tier level validation
- Reference ID format validation

---

## 11. RBAC Integration (ksfraser/rbac)

### 11.1 Module Registration

ksf_FA_Loyalty registers with ksfraser/rbac:
- **record_types**: 'loyalty_account', 'loyalty_transaction', 'coupon'
- **projections**: 'public' (tier, points_balance, transaction_date, coupon_code), 'full' (all fields including points_expiry, coupon_discount, transaction details)
- **allow_invite**: false

### 11.2 Entity Projections

| Entity | PUBLIC Fields | FULL Fields |
|--------|---------------|-------------|
| CustomerLoyalty | tier_level, points_balance, enrollment_date | + points_expiry, lifetime_points, last_activity, linked_customer_id |
| LoyaltyTransaction | points, type (earn/burn), date, description | + reference_order_id, created_by, expiry_date, linked_coupon_id |
| Coupon | code, discount_type, discount_value, status, valid_from/to | + usage_limit, times_used, created_by, exclusion_rules, cost_center |

### 11.3 Access Model

- **Loyalty Admin**: FULL to all loyalty accounts, transactions, coupons — requires PROJECTION_FULL
- **Customer Service**: View customer loyalty tier/balance (PROJECTION_PUBLIC), can issue manual adjustments (can_edit)
- **Marketing**: Create/manage coupons (can_create + can_edit on coupon type), view loyalty tiers for segmentation (PROJECTION_PUBLIC)
- **Customer (via portal)**: View own loyalty account (PROJECTION_PUBLIC via {customerPersonId}_individual team)
- **Finance**: View cost/liability data (PROJECTION_FULL), can_export for reporting

### 11.4 SQL Enforcement

Standard RBAC JOIN pattern against 0_rbac_record_access for all loyalty queries.

### 11.5 Persons Registry

Loyalty accounts link to customers via `customer_person_id` → 0_crm_persons.id. This enables the customer portal access pattern via the person-registry two-legged JOIN.

### 11.6 Soft Delete

- Coupons use soft delete (deactivated → deleted=1)
- Loyalty transactions are append-only (never deleted, audit requirement)
- Hard delete is super-admin only

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-24*
*Author: KSFII Development Team*