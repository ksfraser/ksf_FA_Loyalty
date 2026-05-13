# Test Plan - FA Loyalty Module

## Document Information
- **Module**: ksf_FA_Loyalty
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Introduction

### 1.1 Purpose
This Test Plan defines the testing approach, test scenarios, test data requirements, and pass criteria for the FA Loyalty module.

### 1.2 Scope
Testing covers customer enrollment, points management, transaction tracking, tier management, and menu integration.

### 1.3 Test Environment
- **Platform**: FrontAccounting 2.4.x
- **Database**: MySQL 5.7+
- **PHP Version**: 7.3+
- **Browser**: Chrome/Firefox/Safari (latest)
- **Dependencies**: ksf_FA_CRM module, 0_crm_persons table

---

## 2. Test Strategy

### 2.1 Test Types

| Test Type | Description | Coverage |
|-----------|-------------|----------|
| Unit Testing | Database function testing | includes/*.inc functions |
| Integration Testing | FA hook integration | hooks.php |
| UI Testing | Page rendering and forms | loyalty.php |
| Security Testing | Access control verification | Permission system |

---

## 3. Test Scenarios

### 3.1 Enrollment Tests

#### TC-LOY-001: Enroll New Customer
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-001 |
| Requirement | FR-LOY-001 |
| Priority | High |

**Pre-conditions**:
- Customer ID 123 exists in 0_crm_persons
- Customer not already enrolled

**Test Steps**:
```
1. Call write_customer_loyalty with:
   - customer_person_id: 123
   - points_balance: 0
   - tier_level: 'Bronze'
   - enrollment_date: current date
2. Verify record created with unique loyalty_id
3. Verify points_balance = 0
4. Verify tier_level = 'Bronze'
```

**Test Data**:
```php
$data = [
    'customer_person_id' => 123,
    'points_balance' => 0,
    'tier_level' => 'Bronze',
    'enrollment_date' => '2026-05-13'
];
```

**Pass Criteria**: 
- ✓ loyalty_id auto-generated
- ✓ Customer ID correctly linked
- ✓ Default values applied

---

#### TC-LOY-002: Prevent Duplicate Enrollment
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-002 |
| Requirement | FR-LOY-001 |
| Priority | High |

**Test Steps**:
```
1. Verify customer 123 not enrolled
2. Enroll customer 123
3. Attempt to enroll customer 123 again
4. Verify database constraint prevents duplicate
```

**Expected Result**: Second enrollment fails due to UNIQUE constraint on customer_person_id

**Pass Criteria**: ✓ Duplicate enrollment rejected

---

### 3.2 Points Management Tests

#### TC-LOY-010: Award Points to Customer
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-010 |
| Requirement | FR-LOY-010 |
| Priority | High |

**Pre-conditions**: Customer enrolled with loyalty_id=1, current balance=0

**Test Steps**:
```
1. Call update_loyalty_points(1, 100)
2. Verify points_balance = 100
3. Record transaction with type='EARN'
4. Verify transaction created
```

**Test Data**:
```php
$transaction_data = [
    'loyalty_id' => 1,
    'transaction_type' => 'EARN',
    'points' => 100,
    'reference_type' => 'ORDER',
    'reference_id' => 'ORD-2024-001',
    'description' => 'Purchase reward'
];
```

**Pass Criteria**: 
- ✓ Balance increased by 100
- ✓ Transaction record created
- ✓ Reference tracked

---

#### TC-LOY-011: Redeem Points
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-011 |
| Requirement | FR-LOY-011 |
| Priority | High |

**Pre-conditions**: Customer enrolled with loyalty_id=1, current balance=150

**Test Steps**:
```
1. Call update_loyalty_points(1, -50)
2. Verify points_balance = 100
3. Record transaction with type='REDEEM'
```

**Pass Criteria**: 
- ✓ Balance decreased by 50
- ✓ Transaction created with REDEEM type
- ✓ New balance = 100

---

#### TC-LOY-012: Prevent Negative Balance
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-012 |
| Requirement | FR-LOY-011 |
| Priority | High |

**Pre-conditions**: Customer enrolled with loyalty_id=1, current balance=25

**Test Steps**:
```
1. Call update_loyalty_points(1, -50)
2. Verify operation blocked OR balance = 0 (capped)
```

**Expected Result**: Balance cannot go negative

**Pass Criteria**: ✓ No negative balance possible

---

### 3.3 Transaction Tests

#### TC-LOY-020: Record Loyalty Transaction
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-020 |
| Requirement | FR-LOY-020 |
| Priority | High |

**Test Steps**:
```
1. Call write_loyalty_transaction with transaction data
2. Verify transaction_id auto-generated
3. Verify all fields stored correctly
4. Verify created_at timestamp set
```

**Test Data**:
```php
$data = [
    'loyalty_id' => 1,
    'transaction_type' => 'EARN',
    'points' => 50,
    'reference_type' => 'ORDER',
    'reference_id' => 'ORD-2024-002',
    'description' => 'Bonus points'
];
```

**Pass Criteria**: 
- ✓ Transaction ID generated
- ✓ All data fields correct
- ✓ Timestamp recorded

---

#### TC-LOY-021: View Transaction History
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-021 |
| Requirement | FR-LOY-021 |
| Priority | High |

**Pre-conditions**: 10+ transactions exist for loyalty_id=1

**Test Steps**:
```
1. Call get_loyalty_transactions(1, 50)
2. Verify returns result set
3. Verify default limit of 50 applied
4. Verify ordering: created_at DESC
```

**Pass Criteria**: 
- ✓ All customer transactions returned
- ✓ Sorted by date descending
- ✓ Limited to 50 records default

---

#### TC-LOY-022: Get Specific Transaction
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-022 |
| Requirement | FR-LOY-022 |
| Priority | Medium |

**Test Steps**:
```
1. Create transaction, note transaction_id
2. Call get_transaction(transaction_id)
3. Verify returns correct record
```

**Pass Criteria**: ✓ Correct transaction returned

---

### 3.4 Tier Management Tests

#### TC-LOY-030: Default Tier Assignment
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-030 |
| Requirement | FR-LOY-030 |
| Priority | High |

**Test Steps**:
```
1. Enroll new customer without specifying tier
2. Verify tier_level = 'Bronze'
```

**Pass Criteria**: ✓ Default tier = 'Bronze'

---

#### TC-LOY-031: Update Tier Level
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-031 |
| Requirement | FR-LOY-031 |
| Priority | Medium |

**Pre-conditions**: Customer enrolled with tier='Bronze'

**Test Steps**:
```
1. Call write_customer_loyalty with updated tier
2. Verify tier_level changed to 'Silver'
```

**Test Data**:
```php
$data = ['tier_level' => 'Silver'];
```

**Pass Criteria**: ✓ Tier updated to Silver

---

### 3.5 Menu Integration Tests

#### TC-LOY-040: CRM Menu Integration
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-040 |
| Requirement | FR-LOY-040 |
| Priority | High |

**Test Steps**:
```
1. Login with SA_LOYALTYVIEW permission
2. Navigate to CRM application
3. Verify "Loyalty Program" in Right Panel
4. Click menu item
5. Verify Loyalty page loads
```

**Pass Criteria**: 
- ✓ Menu item visible in CRM
- ✓ Page loads without errors

---

#### TC-LOY-041: Sales Menu Integration
| Attribute | Value |
|-----------|-------|
| Test ID | TC-LOY-041 |
| Requirement | FR-LOY-041 |
| Priority | High |

**Test Steps**:
```
1. Login with SA_LOYALTYVIEW permission
2. Navigate to Sales application
3. Verify "Customer Loyalty" in Left Panel
4. Click menu item
5. Verify Loyalty page loads
```

**Pass Criteria**: 
- ✓ Menu item visible in Sales
- ✓ Page loads without errors

---

## 4. Security Tests

### TC-LOY-SEC-001: View Permission Only
| Test ID | TC-LOY-SEC-001 |
|---------|---------------|
| Priority | High |

**Test Steps**:
```
1. Login as user with SA_LOYALTYVIEW only
2. Navigate to Loyalty Program
3. Verify View options available
4. Verify Create/Modify options NOT visible
```

**Pass Criteria**: 
- ✓ View operations accessible
- ✓ Modification options hidden

---

### TC-LOY-SEC-002: No Access Without Permission
| Test ID | TC-LOY-SEC-002 |
|---------|---------------|

**Test Steps**:
```
1. Login as user without SA_LOYALTYVIEW
2. Attempt to access Loyalty menu
3. Verify access denied
```

**Pass Criteria**: 
- ✓ Menu not visible OR
- ✓ Access denied message shown

---

## 5. Test Data Setup

### 5.1 Required Test Data

| Entity | Quantity | Notes |
|--------|----------|-------|
| CRM Persons | 10+ | Customers in CRM |
| Loyalty Records | 5+ | Various tiers and balances |
| Transactions | 50+ | Mix of EARN/REDEEM/ADJUST |

### 5.2 Test Data Script

```sql
-- Insert test customers
INSERT INTO 0_crm_persons (name, email) VALUES
('Test Customer 1', 'test1@example.com'),
('Test Customer 2', 'test2@example.com'),
('Test Customer 3', 'test3@example.com');

-- Insert loyalty records
INSERT INTO fa_customer_loyalty (customer_person_id, points_balance, tier_level, enrollment_date) VALUES
(1, 500, 'Bronze', CURDATE()),
(2, 1500, 'Silver', CURDATE()),
(3, 5000, 'Gold', CURDATE());
```

---

## 6. Pass Criteria Summary

| Category | Criteria | Target |
|----------|----------|--------|
| Functional | All test cases pass | 100% |
| Security | Access control verified | 100% |
| Data Integrity | Foreign keys, constraints valid | 100% |
| Performance | Operations complete within targets | 100% |

---

## 7. Performance Tests

### TC-LOY-PERF-001: Loyalty Lookup Performance
| Test ID | TC-LOY-PERF-001 |
|---------|-----------------|
| Target | < 50ms |

**Test Steps**:
```
1. Call get_loyalty_by_customer with valid person_id
2. Measure execution time
```

**Pass Criteria**: ✓ Lookup completes < 50ms

---

### TC-LOY-PERF-002: Transaction History Load
| Test ID | TC-LOY-PERF-002 |
|---------|-----------------|
| Target | < 100ms |

**Test Steps**:
```
1. Call get_loyalty_transactions with limit=50
2. Measure execution time
```

**Pass Criteria**: ✓ History loads < 100ms

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*