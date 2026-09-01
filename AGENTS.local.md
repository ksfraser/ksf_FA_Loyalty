<!-- Repo-specific appendix to the shared AGENTS.md. Generic conventions live in AGENTS_ARCH.md (hardlinked). -->

# AGENTS.md - ksf_FA_Loyalty
## Architecture Overview
This repository implements **Customer Loyalty Program** similar to OpenProject's OKR rewards system - tracks points, tiers, and transactions.
## Repository Structure
```
ksf_FA_Loyalty/
├── sql/                    # Database schemas (FA TB_PREF tables)
│   ├── fa_customer_loyalty.sql
│   ├── fa_loyalty_transactions.sql
│   └── fa_loyalty_tiers.sql
├── includes/              # FA-specific DB classes
│   ├── loyalty_db.inc
│   ├── loyalty_transactions_db.inc
│   └── ...
├── pages/                 # UI pages (FA admin)
├── hooks.php              # FA module hooks (uses update_databases())
├── composer.json
└── ProjectDocs/           # Project documentation
    ├── Requirements.md
    ├── RTM.md            # Requirements Traceability Matrix
    ├── BABOK.md         # Business Analysis Body of Knowledge
    └── UML.md           # UML diagrams
```
## Dependencies
- **ksf_FA_Loyalty_Core** (business logic - framework-agnostic)
- **ksf_FA_CRM** (customer contacts)
- **FrontAccounting 2.4+** (FA core)
