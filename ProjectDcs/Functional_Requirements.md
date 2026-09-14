# Functional Requirements - FA Loyalty Module

## Document Information
- **Module**: ksf_FA_Loyalty
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-24

---

## 1. Introduction

### 1.1 Purpose
This document defines the functional requirements for the FA Loyalty module, a FrontAccounting platform adapter that provides customer loyalty program functionality including enrollment, points tracking, tier management, and transaction history.

### 1.2 Scope
These requirements cover all functional aspects of the loyalty system including customer enrollment, points management, transaction logging, and tier level administration.

---

## 2. Functional Requirements

### 2.1 Customer Enrollment

#### FR-LOY-001: Enroll Customer in Loyalty Program
- **Description**: System administrators can enroll customers in the loyalty program
- **Preconditions**: Customer exists in CRM (0_crm_persons), not already enrolled
- **Trigger**: Select customer and submit enrollment form
- **Fields**:
  | Field | Type | Required | Validation |
  |-------|------|----------|-------------|
  | Customer Person ID | INT | Yes | Valid FK to 0_crm_persons |
  | Points Balance | INT | No | Default 0, non-negative |
  | Tier Level | VARCHAR(20) | No | Default 'Bronze' |
  | Enrollment Date | DATE | Auto | Current date |
- **Expected Results**: New loyalty record created with unique loyalty_id
- **Priority**: High

#### FR-LOY-002: View Customer Loyalty Status
- **Description**: Users can view loyalty information for any enrolled customer
- **Preconditions**: Customer enrolled in loyalty program
- **Trigger**: Search or select customer from CRM
- **Displayed Fields**:
  - Loyalty ID
  - Customer Name (from CRM)
  - Email (from CRM)
  - Points Balance
  - Current Tier Level
  - Enrollment Date
- **Expected Results**: All loyalty data displayed correctly
- **Priority**: High

#### FR-LOY-003: Lookup Loyalty by Customer
- **Description**: System can retrieve loyalty record by CRM person ID
- **Preconditions**: Customer person ID provided
- **Implementation**: get_loyalty_by_customer(person_id)
- **Expected Results**: Returns loyalty record array or null
- **Priority**: High

#### FR-LOY-004: Remove Customer from Loyalty Program
- **Description**: Admins can remove customer from loyalty program
- **Preconditions**: Customer enrolled, user has SA_LOYALTYCREATE
- **Trigger**: Delete loyalty record action
- **Expected Results**: Loyalty record deleted, transaction history preserved
- **Priority**: Medium

---

### 2.2 Points Management

#### FR-LOY-010: Add Points to Customer Balance
- **Description**: Admins can add points to customer loyalty balance
- **Preconditions**: Customer enrolled, user has SA_LOYALTYCREATE
- **Trigger**: Points adjustment action
- **Implementation**: update_loyalty_points(loyalty_id, positive_change)
- **Expected Results**:
  - Points balance increased by specified amount
  - Transaction record created with type 'EARN'
  - Transaction includes reference to source document
- **Priority**: High

#### FR-LOY-011: Deduct Points from Customer Balance
- **Description**: Admins can deduct/void points from customer balance
- **Preconditions**: Customer enrolled, sufficient points balance
- **Trigger**: Points redemption or adjustment action
- **Implementation**: update_loyalty_points(loyalty_id, negative_change)
- **Expected Results**:
  - Points balance decreased by specified amount
  - Transaction record created with type 'REDEEM' or 'ADJUST'
  - Balance cannot go negative
- **Priority**: High

#### FR-LOY-012: View Points Balance
- **Description**: Users can view current points balance
- **Preconditions**: Customer enrolled
- **Trigger**: View customer loyalty status
- **Expected Results**: Current points balance displayed accurately
- **Priority**: High

---

### 2.3 Transaction Management

#### FR-LOY-020: Record Loyalty Transaction
- **Description**: System records all loyalty-related transactions
- **Preconditions**: Customer enrolled
- **Trigger**: Points earned, redeemed, or adjusted
- **Fields**:
  | Field | Type | Required | Notes |
  |-------|------|----------|-------|
  | loyalty_id | INT | Yes | FK to customer loyalty |
  | transaction_type | VARCHAR | Yes | EARN/REDEEM/ADJUST/EXPIRE |
  | points | INT | Yes | Positive or negative |
  | reference_type | VARCHAR | No | ORDER/INVOICE/ADMIN/MANUAL |
  | reference_id | VARCHAR | No | External reference |
  | description | TEXT | No | Human-readable description |
- **Expected Results**: Transaction record created with timestamp
- **Priority**: High

#### FR-LOY-021: View Transaction History
- **Description**: Users can view loyalty transaction history for customer
- **Preconditions**: Customer enrolled
- **Trigger**: View customer loyalty details
- **Implementation**: get_loyalty_transactions(loyalty_id, limit)
- **Default Limit**: 50 transactions
- **Sorting**: Most recent first (ORDER BY created_at DESC)
- **Expected Results**: List of transactions with type, points, date, description
- **Priority**: High

#### FR-LOY-022: Get Specific Transaction
- **Description**: Users can retrieve details of specific transaction
- **Preconditions**: Transaction ID known
- **Implementation**: get_transaction(transaction_id)
- **Expected Results**: Single transaction record returned
- **Priority**: Medium

#### FR-LOY-023: Void/Delete Transaction
- **Description**: Admins can void loyalty transactions
- **Preconditions**: Transaction exists, user has SA_LOYALTYCREATE
- **Trigger**: Admin action to remove transaction
- **Implementation**: delete_loyalty_transaction(transaction_id)
- **Expected Results**: Transaction removed from history
- **Priority**: Medium

---

### 2.4 Tier Management

#### FR-LOY-030: Default Tier Assignment
- **Description**: New enrollments receive Bronze tier by default
- **Implementation**: Tier field default value in database
- **Expected Results**: New customers get tier_level = 'Bronze'
- **Priority**: High

#### FR-LOY-031: Update Tier Level
- **Description**: Admins can change customer tier level
- **Preconditions**: Customer enrolled, user has SA_LOYALTYCREATE
- **Trigger**: Tier change action
- **Valid Values**: Bronze, Silver, Gold, Platinum
- **Expected Results**: Tier level updated, no balance change
- **Priority**: Medium

#### FR-LOY-032: View Current Tier
- **Description**: Users can see current tier level
- **Preconditions**: Customer enrolled
- **Expected Results**: Tier displayed with visual indicator
- **Priority**: High

---

### 2.5 Menu Integration

#### FR-LOY-040: CRM Menu Integration
- **Description**: Loyalty access via CRM application menu
- **Location**: CRM > Right Panel Functions
- **Menu Item**: "Loyalty Program"
- **Security Required**: SA_LOYALTYVIEW
- **Priority**: High

#### FR-LOY-041: Sales Menu Integration
- **Description**: Loyalty access via Sales application menu
- **Location**: Sales > Left Panel Functions
- **Menu Item**: "Customer Loyalty"
- **Security Required**: SA_LOYALTYVIEW
- **Priority**: High

---

## 3. Non-Functional Requirements

### 3.1 Performance
- Loyalty lookup by customer ID: < 50ms
- Transaction history load (50 records): < 100ms
- Points update operation: < 50ms

### 3.2 Security
- All database queries use db_escape() for SQL injection prevention
- Access controlled via FA security areas
- Foreign key constraint prevents orphaned records

### 3.3 Data Integrity
- Points balance non-negative enforced at application level
- Unique constraint on customer_person_id prevents duplicate enrollment
- Transaction audit trail preserved

### 3.4 Compatibility
- Compatible with FrontAccounting 2.4.x
- PHP 7.3+ compatibility
- Follows FA coding standards

---

## 4. Requirements Traceability

| Requirement ID | Description | Test Case | Status |
|----------------|-------------|-----------|--------|
| FR-LOY-001 | Enroll Customer | TC-LOY-001 | ✓ |
| FR-LOY-002 | View Loyalty Status | TC-LOY-002 | ✓ |
| FR-LOY-003 | Lookup by Customer | TC-LOY-003 | ✓ |
| FR-LOY-010 | Add Points | TC-LOY-010 | ✓ |
| FR-LOY-011 | Deduct Points | TC-LOY-011 | ✓ |
| FR-LOY-020 | Record Transaction | TC-LOY-020 | ✓ |
| FR-LOY-021 | View History | TC-LOY-021 | ✓ |
| FR-LOY-030 | Default Tier | TC-LOY-030 | ✓ |
| FR-LOY-040 | CRM Menu | TC-LOY-040 | ✓ |
| FR-LOY-041 | Sales Menu | TC-LOY-041 | ✓ |

---

## 5. Database Schema

### 5.1 fa_customer_loyalty
```sql
CREATE TABLE IF NOT EXISTS `fa_customer_loyalty` (
    `loyalty_id` INT(11) NOT NULL AUTO_INCREMENT,
    `customer_person_id` INT(11) NOT NULL COMMENT 'FK to 0_crm_persons',
    `points_balance` INT(11) DEFAULT 0,
    `tier_level` VARCHAR(20) DEFAULT 'Bronze',
    `enrollment_date` DATE DEFAULT NULL,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`loyalty_id`),
    UNIQUE KEY `idx_customer` (`customer_person_id`),
    CONSTRAINT `fk_loyalty_customer` FOREIGN KEY (`customer_person_id`) 
        REFERENCES `0_crm_persons`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
```

### 5.2 fa_loyalty_transactions
```sql
CREATE TABLE IF NOT EXISTS `fa_loyalty_transactions` (
    `transaction_id` INT(11) NOT NULL AUTO_INCREMENT,
    `loyalty_id` INT(11) NOT NULL,
    `transaction_type` VARCHAR(20) NOT NULL COMMENT 'EARN/REDEEM/ADJUST/EXPIRE',
    `points` INT(11) NOT NULL,
    `reference_type` VARCHAR(20) DEFAULT NULL COMMENT 'ORDER/INVOICE/ADMIN/MANUAL',
    `reference_id` VARCHAR(100) DEFAULT NULL,
    `description` TEXT,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`transaction_id`),
    KEY `idx_loyalty` (`loyalty_id`),
    KEY `idx_type` (`transaction_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
```

---

## 6. RBAC Integration Requirements

### 6.1 RBAC Integration

#### FR-LOY-007: RBAC Integration

**Description:** All loyalty queries use the RBAC JOIN pattern against `0_rbac_record_access` to enforce record-level access control for loyalty accounts, transactions, and coupons.

**Acceptance Criteria:**
- [ ] Every SELECT query on loyalty_accounts, loyalty_transactions, and coupons includes RBAC JOIN
- [ ] Queries filter by `ra.module = 'loyalty'` and appropriate `ra.record_type`
- [ ] Inactive records are excluded (`ra.inactive = 0`)
- [ ] User must be a member of a team with `can_view = 1`

---

#### FR-LOY-008: DTO Projections

**Description:** Loyalty account, transaction, and coupon entities define PUBLIC and FULL field projections for RBAC-scoped data access.

| Entity | PUBLIC Fields | FULL Fields |
|--------|---------------|-------------|
| CustomerLoyalty | tier_level, points_balance, enrollment_date | + points_expiry, lifetime_points, last_activity, linked_customer_id |
| LoyaltyTransaction | points, type (earn/burn), date, description | + reference_order_id, created_by, expiry_date, linked_coupon_id |
| Coupon | code, discount_type, discount_value, status, valid_from/to | + usage_limit, times_used, created_by, exclusion_rules, cost_center |

**Acceptance Criteria:**
- [ ] PUBLIC projection accessible with PROJECTION_PUBLIC permission
- [ ] FULL projection requires PROJECTION_FULL permission
- [ ] PUBLIC fields exclude financial/cost data and internal references

---

#### FR-LOY-009: Soft Delete

**Description:** Coupons use soft delete (`deleted=1`) when deactivated. Loyalty transactions are append-only (never deleted).

**Acceptance Criteria:**
- [ ] Deactivated coupons set `deleted=1` instead of hard-deleting rows
- [ ] Soft-deleted coupons excluded from default queries
- [ ] Loyalty transactions are append-only — no delete operation permitted
- [ ] Hard delete is super-admin only

---

#### FR-LOY-010: Persons Registry

**Description:** Loyalty accounts link to customers via `customer_person_id` → `0_crm_persons.id`, enabling the customer portal access pattern via the person-registry two-legged JOIN.

**Acceptance Criteria:**
- [ ] Foreign key from `fa_customer_loyalty.customer_person_id` to `0_crm_persons.id`
- [ ] Customer portal uses `{customerPersonId}_individual` team pattern for self-service access
- [ ] RBAC JOIN resolves portal access via person-registry relationship

---

#### FR-LOY-011: Audit

**Description:** Key loyalty operations are logged to the RBAC audit log for compliance and traceability.

**Audited Events:**
- Points adjustments (manual earn/burn)
- Coupon creation and usage
- Tier level changes
- Enrollment and deactivation

**Acceptance Criteria:**
- [ ] All audited events create audit log entries
- [ ] Audit entries include user ID, timestamp, and before/after values
- [ ] Audit log is append-only (no deletion or modification)

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-24*
*Author: KSFII Development Team*