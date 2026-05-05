-- fa_loyalty_transactions table
-- Track points earned/redeemed

CREATE TABLE IF NOT EXISTS `fa_loyalty_transactions` (
    `transaction_id` INT(11) NOT NULL AUTO_INCREMENT,
    `loyalty_id` INT(11) NOT NULL,
    `transaction_type` VARCHAR(20) NOT NULL COMMENT 'EARN, REDEEM, ADJUST',
    `points` INT(11) NOT NULL COMMENT 'Positive for earn, negative for redeem',
    `reference_type` VARCHAR(20) DEFAULT NULL COMMENT 'INVOICE, MANUAL, etc.',
    `reference_id` INT(11) DEFAULT NULL,
    `description` VARCHAR(255) DEFAULT NULL,
    `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`transaction_id`),
    KEY `idx_loyalty` (`loyalty_id`),
    KEY `idx_type` (`transaction_type`),
    CONSTRAINT `fk_loyalty_trans` FOREIGN KEY (`loyalty_id`) REFERENCES `fa_customer_loyalty`(`loyalty_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
