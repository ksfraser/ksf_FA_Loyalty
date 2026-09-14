# UAT Plan - FA Loyalty Module

## Document Information
- **Module**: ksf_FA_Loyalty
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13

---

## 1. Introduction

### 1.1 Purpose
This User Acceptance Testing (UAT) Plan defines the objectives, scenarios, and success criteria for validating the FA Loyalty module in a production-like environment.

### 1.2 Scope
UAT covers customer enrollment in loyalty program, points management, transaction tracking, tier administration, and menu integration with CRM and Sales applications.

### 1.3 Stakeholders
| Role | Responsibility |
|------|----------------|
| Project Manager | UAT oversight and sign-off |
| Business Analyst | Requirements validation |
| End Users (Sales/CRM staff) | Functional testing |
| IT Administrator | Technical validation |
| QA Lead | Test execution oversight |

---

## 2. UAT Objectives

### 2.1 Primary Objectives
1. **Functional Validation**: Verify all business requirements are implemented correctly
2. **Points Accuracy**: Ensure points calculations are accurate
3. **Transaction Integrity**: Verify complete audit trail
4. **Integration Verification**: Confirm CRM and Sales integration works
5. **Security Compliance**: Verify access controls function correctly

### 2.2 Success Criteria
- All critical test scenarios pass
- All high-priority test scenarios pass
- No blocker or critical defects open
- Points calculations verified accurate
- User sign-off obtained from all stakeholder groups

---

## 3. Test Scenarios

### 3.1 Scenario Set 1: Menu Access

#### UAT-LOY-S1-001: Access Loyalty from CRM
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S1-001 |
| Priority | Critical |
| Precondition | User authenticated with SA_LOYALTYVIEW |

**Steps**:
```
1. Log into FrontAccounting
2. Navigate to CRM application module
3. Locate "Loyalty Program" in right panel
4. Click on "Loyalty Program"
5. Verify Loyalty page loads successfully
```

**Expected Result**: Loyalty program interface accessible via CRM

**Pass Criteria**: 
- ✓ Menu item visible in CRM right panel
- ✓ Page loads without errors
- ✓ Loyalty interface displayed

---

#### UAT-LOY-S1-002: Access Loyalty from Sales
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S1-002 |
| Priority | Critical |

**Steps**:
```
1. Log into FrontAccounting
2. Navigate to Sales application module
3. Locate "Customer Loyalty" in left panel
4. Click on "Customer Loyalty"
5. Verify Loyalty page loads successfully
```

**Pass Criteria**: 
- ✓ Menu item visible in Sales left panel
- ✓ Page loads without errors

---

### 3.2 Scenario Set 2: Customer Enrollment

#### UAT-LOY-S2-001: Enroll New Customer
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S2-001 |
| Priority | Critical |
| Precondition | User has SA_LOYALTYCREATE permission |

**Steps**:
```
1. Navigate to Loyalty Program
2. Click "Enroll New Customer"
3. Search for customer: "Test Customer 1"
4. Select customer from results
5. Verify customer details displayed
6. Click "Enroll"
7. Verify success notification
8. Verify customer appears in loyalty list with:
   - Points Balance: 0
   - Tier: Bronze
   - Enrollment Date: Today
```

**Test Data**:
```
Customer: Test Customer 1 (ID: 1 in CRM)
Expected Tier: Bronze
Expected Points: 0
```

**Pass Criteria**: 
- ✓ Enrollment form accessible
- ✓ Customer selected successfully
- ✓ Enrollment successful
- ✓ Loyalty record created with correct defaults

---

#### UAT-LOY-S2-002: View Loyalty Status
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S2-002 |
| Priority | High |

**Preconditions**: Customer enrolled with known loyalty_id

**Steps**:
```
1. Navigate to Loyalty Program
2. Search for enrolled customer
3. Click on customer record
4. Verify displayed information:
   - Customer Name
   - Email Address
   - Loyalty ID
   - Points Balance
   - Current Tier
   - Enrollment Date
```

**Pass Criteria**: 
- ✓ All fields displayed correctly
- ✓ Data matches database record
- ✓ Customer name/email from CRM joined correctly

---

#### UAT-LOY-S2-003: Prevent Duplicate Enrollment
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S2-003 |
| Priority | High |

**Steps**:
```
1. Enrolled customer from S2-001 (already enrolled)
2. Attempt to enroll same customer again
3. Observe system response
```

**Expected Result**: System prevents duplicate enrollment

**Pass Criteria**: 
- ✓ Error message displayed
- ✓ Database constraint prevents duplicate
- ✓ No orphaned or duplicate records

---

### 3.3 Scenario Set 3: Points Management

#### UAT-LOY-S3-001: Award Points
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S3-001 |
| Priority | Critical |

**Preconditions**: Customer enrolled with balance=0

**Steps**:
```
1. Navigate to customer loyalty detail
2. Click "Award Points"
3. Enter points amount: 100
4. Select reference type: "Purchase"
5. Enter reference: "ORD-2026-001"
6. Add description: "Purchase reward"
7. Click "Award"
8. Verify points added to balance
9. Verify success notification
10. Verify balance now shows: 100
```

**Test Data**:
```
Points to Award: 100
Reference Type: ORDER
Reference ID: ORD-2026-001
```

**Pass Criteria**: 
- ✓ Points form accessible
- ✓ Validation prevents non-positive amounts
- ✓ Points added correctly
- ✓ New balance = 100
- ✓ Success message displayed

---

#### UAT-LOY-S3-002: Redeem Points
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S3-002 |
| Priority | Critical |

**Preconditions**: Customer has 100+ points

**Steps**:
```
1. Navigate to customer loyalty detail
2. Verify current balance: 100
3. Click "Redeem Points"
4. Enter points to redeem: 25
5. Select redemption reason: "Discount"
6. Click "Redeem"
7. Verify points deducted from balance
8. Verify balance now shows: 75
```

**Pass Criteria**: 
- ✓ Redemption form accessible
- ✓ Points deducted correctly
- ✓ New balance = 75

---

#### UAT-LOY-S3-003: Prevent Excessive Redemption
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S3-003 |
| Priority | High |

**Steps**:
```
1. Customer has 50 points
2. Attempt to redeem 100 points
3. Observe system response
```

**Expected Result**: System prevents over-redemption

**Pass Criteria**: 
- ✓ Error message shown
- ✓ Redemption blocked
- ✓ Balance unchanged at 50

---

#### UAT-LOY-S3-004: Prevent Negative Balance
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S3-004 |
| Priority | High |

**Steps**:
```
1. Customer has 30 points
2. Redeem 50 points
3. Observe balance result
```

**Expected Result**: Balance cannot go negative

**Pass Criteria**: 
- ✓ Balance capped at 0
- ✓ No negative values in system

---

### 3.4 Scenario Set 4: Transaction History

#### UAT-LOY-S4-001: View Transaction History
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S4-001 |
| Priority | Critical |

**Preconditions**: Customer has multiple transactions

**Steps**:
```
1. Navigate to customer loyalty detail
2. Click "View History" or "Transactions" tab
3. Verify transaction list displayed
4. Verify each transaction shows:
   - Date/Time
   - Transaction Type (EARN/REDEEM/ADJUST)
   - Points Amount (+/-)
   - Description
   - Reference
5. Verify transactions ordered by date (newest first)
```

**Pass Criteria**: 
- ✓ Transaction list accessible
- ✓ All fields displayed
- ✓ Chronological order correct
- ✓ Earned points positive, redeemed negative

---

#### UAT-LOY-S4-002: Transaction Audit Trail
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S4-002 |
| Priority | High |

**Steps**:
```
1. Perform points transaction
2. View transaction history
3. Verify transaction recorded with:
   - Correct type (EARN/REDEEM/ADJUST)
   - Correct points amount
   - Reference information
   - Timestamp
```

**Pass Criteria**: 
- ✓ Complete transaction recorded
- ✓ Audit trail complete

---

### 3.5 Scenario Set 5: Tier Management

#### UAT-LOY-S5-001: Default Tier on Enrollment
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S5-001 |
| Priority | High |

**Steps**:
```
1. Enroll new customer
2. View new customer loyalty record
3. Verify tier level
```

**Expected Result**: New customers receive Bronze tier

**Pass Criteria**: 
- ✓ Default tier = 'Bronze'
- ✓ No manual tier assignment during enrollment

---

#### UAT-LOY-S5-002: Update Tier Level
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S5-002 |
| Priority | Medium |

**Preconditions**: Admin with SA_LOYALTYCREATE permission

**Steps**:
```
1. Navigate to customer loyalty detail
2. Click "Change Tier"
3. Select new tier: "Silver"
4. Click "Update"
5. Verify tier changed to Silver
6. Verify points balance unchanged
```

**Pass Criteria**: 
- ✓ Tier updated successfully
- ✓ Points balance unaffected

---

### 3.6 Scenario Set 6: Security and Access Control

#### UAT-LOY-S6-001: View-Only Access
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S6-001 |
| Priority | Critical |

**Steps**:
```
1. Login as staff user with SA_LOYALTYVIEW only (no SA_LOYALTYCREATE)
2. Navigate to Loyalty Program
3. Verify can view customer loyalty status
4. Verify cannot access:
   - Enroll new customer
   - Award points
   - Redeem points
   - Change tier
   - Delete loyalty record
```

**Expected Result**: Read-only access granted

**Pass Criteria**: 
- ✓ View operations work
- ✓ Modification buttons hidden or disabled
- ✓ No modification possible

---

#### UAT-LOY-S6-002: Full Admin Access
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S6-002 |
| Priority | Critical |

**Steps**:
```
1. Login as admin with SA_LOYALTYCREATE permission
2. Navigate to Loyalty Program
3. Verify all functions accessible:
   - View loyalty status
   - Enroll customers
   - Award points
   - Redeem points
   - Adjust points
   - Change tier
```

**Expected Result**: Full CRUD access granted

**Pass Criteria**: 
- ✓ All functions accessible
- ✓ No permission errors

---

#### UAT-LOY-S6-003: Deny Access Without Permission
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S6-003 |
| Priority | Critical |

**Steps**:
```
1. Login as user without SA_LOYALTYVIEW permission
2. Attempt to access Loyalty Program
```

**Expected Result**: Access denied

**Pass Criteria**: 
- ✓ Menu item not visible OR
- ✓ Access denied error shown

---

### 3.7 Scenario Set 7: Data Integrity

#### UAT-LOY-S7-001: Foreign Key Constraint
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S7-001 |
| Priority | High |

**Steps**:
```
1. Attempt to enroll non-existent customer (invalid CRM person ID)
2. Observe system response
```

**Expected Result**: System prevents enrollment of invalid customer

**Pass Criteria**: 
- ✓ Validation error shown
- ✓ Invalid record not created

---

#### UAT-LOY-S7-002: Transaction Reference Integrity
| Attribute | Value |
|-----------|-------|
| Scenario ID | UAT-LOY-S7-002 |
| Priority | Medium |

**Steps**:
```
1. Award points with reference
2. Verify transaction stores reference correctly
3. Verify reference displayed in history
```

**Pass Criteria**: 
- ✓ Reference stored in database
- ✓ Reference displayed in UI

---

## 4. Defect Tracking

### 4.1 Defect Severity Definitions

| Severity | Definition | Example |
|----------|------------|---------|
| Critical | Points calculation error, data loss | Points credited wrong, balance corruption |
| High | Major function broken | Cannot enroll, cannot redeem |
| Medium | Function impaired | Slow load, minor display issue |
| Low | Cosmetic issue | Typo, formatting |

### 4.2 Critical Test Data

| Test Case | Points Before | Action | Points After |
|-----------|---------------|--------|--------------|
| TC-1 | 0 | Award 100 | 100 |
| TC-2 | 100 | Redeem 50 | 50 |
| TC-3 | 50 | Redeem 100 (exceeds) | 50 (no change) |
| TC-4 | 50 | Award 25 | 75 |

---

## 5. Sign-Off Criteria

### 5.1 Release Criteria

| Criterion | Target | Status |
|-----------|--------|--------|
| Critical scenarios pass | 100% | Required |
| High priority scenarios pass | 100% | Required |
| Points calculations verified | 100% | Required |
| Security controls verified | 100% | Required |
| Critical defects open | 0 | Required |
| High defects open | 0 | Required |

### 5.2 Points Verification Checklist

| Test | Expected | Actual | Verified |
|------|----------|--------|----------|
| New enrollment balance | 0 | ___ | ☐ |
| Award 100 points | 100 | ___ | ☐ |
| Redeem 50 points | 50 | ___ | ☐ |
| Award 25 more | 75 | ___ | ☐ |

---

## 6. Test Execution Timeline

| Phase | Activity | Duration |
|-------|----------|----------|
| Day 1 AM | UAT Planning & Environment Setup | 4 hours |
| Day 1 PM | Enrollment & Menu Access Testing | 4 hours |
| Day 2 AM | Points Management Testing | 4 hours |
| Day 2 PM | Transaction & Tier Testing | 4 hours |
| Day 3 AM | Security Testing | 4 hours |
| Day 3 PM | Regression & Final Verification | 4 hours |
| Day 4 | Sign-off and Handover | 4 hours |

---

## 7. Sign-Off Template

```
UAT Sign-Off Form - FA Loyalty Module

Module: ksf_FA_Loyalty
Version: 1.0.0
Test Period: [Start Date] - [End Date]

Test Summary:
- Total Scenarios: [X]
- Passed: [X]
- Failed: [X]
- Blocked: [X]

Critical/High Defects: [List or "None"]

Points Verification:
- Points calculations verified accurate: [Yes/No]
- Transaction audit trail complete: [Yes/No]

User Acceptance Decision:
[ ] APPROVED - Module meets acceptance criteria
[ ] APPROVED WITH CONDITIONS - Minor issues noted
[ ] REJECTED - Major issues require resolution

Comments:
_____________________________________________________________

Signee Name: _________________________
Role: _________________________
Date: _________________________
Signature: _________________________
```

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*