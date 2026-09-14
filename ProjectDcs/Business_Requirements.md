# Business Requirements - FA Loyalty Module

## Document Information
- **Module**: ksf_FA_Loyalty
- **Type**: FrontAccounting Platform Adapter
- **Version**: 1.0.0
- **Date**: 2026-05-13
- **Status**: Implemented

---

## 1. Project Overview

### 1.1 Module Purpose
The FA Loyalty module is a FrontAccounting platform adapter that integrates customer loyalty program functionality with the FA CRM system. It enables organizations to reward customers through points-based loyalty programs, track transaction history, and manage tier levels.

### 1.2 Problem Statement
Organizations require mechanisms to:
- Build customer loyalty and retention through reward programs
- Track customer purchases and award points automatically
- Provide tiered benefits based on customer value
- Manage promotional coupons and special offers
- Integrate loyalty rewards with sales transactions

This module addresses these needs by providing a complete loyalty management system within FrontAccounting.

### 1.3 Module Type
**Platform Adapter with Business Logic**: This module provides FA-specific database adapters and UI for loyalty management, consuming business logic from the CRM core.

---

## 2. Scope

### 2.1 In Scope
- Customer enrollment in loyalty program
- Points balance tracking and management
- Transaction history for loyalty activity
- Tier level management (Bronze, Silver, Gold, Platinum)
- Points earning rules based on purchases
- Points redemption capabilities
- Customer loyalty lookup from CRM
- Menu integration with CRM and Sales applications

### 2.2 Out of Scope
- Advanced tier rules and promotions
- Points expiration management
- Gift card functionality
- Multi-currency point values
- Automated point calculation hooks
- Loyalty reporting and analytics dashboard
- Email/SMS notifications
- Integration with external loyalty programs

### 2.3 Dependencies
| Dependency | Type | Purpose |
|------------|------|---------|
| FrontAccounting 2.4.x | Platform | Target ERP system |
| ksf_FA_CRM | CRM Module | Customer data integration |
| 0_crm_persons table | Database | Customer reference |

---

## 3. Features

### 3.1 Core Features

#### F1: Customer Enrollment
- Enroll customers in loyalty program
- Automatic tier assignment based on criteria
- Enrollment date tracking
- Unique loyalty ID per customer

#### F2: Points Management
- Track points balance per customer
- Award points for purchases
- Deduct points for redemptions
- Points history transaction log

#### F3: Tier Management
- Bronze tier (entry level)
- Silver tier
- Gold tier
- Platinum tier (highest)
- Tier upgrade/downgrade tracking

#### F4: Transaction Tracking
- Record all loyalty transactions
- Link transactions to sales orders/invoices
- Transaction types: EARN, REDEEM, ADJUST, EXPIRE
- Transaction descriptions and references

### 3.2 User Roles and Permissions

| Role | SA_LOYALTYVIEW | SA_LOYALTYCREATE |
|------|----------------|------------------|
| Guest | No access | No access |
| Staff | View loyalty info | No |
| Admin | Full access | Full access |

---

## 4. Functional Overview

### 4.1 Data Model

#### Tables Created
```
fa_customer_loyalty
├── loyalty_id (PK, INT)
├── customer_person_id (FK → 0_crm_persons)
├── points_balance (INT)
├── tier_level (VARCHAR: Bronze/Silver/Gold/Platinum)
├── enrollment_date (DATE)
└── created_at (TIMESTAMP)

fa_loyalty_transactions
├── transaction_id (PK, INT)
├── loyalty_id (FK → fa_customer_loyalty)
├── transaction_type (VARCHAR: EARN/REDEEM/ADJUST/EXPIRE)
├── points (INT: positive/negative)
├── reference_type (VARCHAR: ORDER/INVOICE/ADMIN/MANUAL)
├── reference_id (VARCHAR)
├── description (TEXT)
└── created_at (TIMESTAMP)
```

### 4.2 Menu Integration

| Location | Function | Security | Menu Type |
|----------|----------|----------|-----------|
| CRM > Right | Loyalty Program | SA_LOYALTYVIEW | Entry |
| Sales > Left | Customer Loyalty | SA_LOYALTYVIEW | Entry |

---

## 5. Integration Architecture

### 5.1 Module Architecture
```
FrontAccounting Core
        ↓
ksf_FA_CRM (Customer Data)
        ↓
ksf_FA_Loyalty (Loyalty Adapter)
    ├── hooks.php (FA Integration)
    ├── includes/loyalty_db.inc (Customer Loyalty DB)
    ├── includes/loyalty_transactions_db.inc (Transaction DB)
    └── includes/coupons_db.inc (Coupon DB - future)
```

### 5.2 FrontAccounting Integration Points

| Hook | Purpose |
|------|---------|
| install_options() | Register menu items in CRM and Sales |
| install_access() | Define security areas |
| activate_extension() | Create loyalty tables from SQL files |
| db_prevoid() | Handle voiding of loyalty transactions |

---

## 6. Security Considerations

### 6.1 Access Control
- Security section: SS_LOYALTY (118 << 8)
- Two security areas: SA_LOYALTYVIEW, SA_LOYALTYCREATE
- Foreign key constraint to CRM persons table

### 6.2 Data Validation
- SQL injection prevention via db_escape()
- XSS prevention through FA output functions
- Foreign key validation for customer_person_id
- Points balance non-negative validation

---

## 7. Performance Requirements

| Metric | Target |
|--------|--------|
| Loyalty lookup by customer | < 50ms |
| Transaction history load | < 100ms |
| Points update | < 50ms |

---

## 8. Future Enhancements

| Feature | Priority | Description |
|---------|----------|-------------|
| Points expiration | Medium | Auto-expire old points |
| Tier-based discounts | Medium | Auto-apply discounts by tier |
| Loyalty analytics | Low | Dashboard with KPIs |
| Automated point awards | High | Hook into sales transactions |
| Email notifications | Low | Points earned/redeemed alerts |

---

*Document Version: 1.0.0*
*Last Updated: 2026-05-13*
*Author: KSFII Development Team*