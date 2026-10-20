<?php

declare(strict_types=1);

namespace Ksfraser\Tests\Unit\FA\Loyalty;

use PHPUnit\Framework\TestCase;

/**
 * The loyalty tables were created UNPREFIXED (`fa_customer_loyalty`,
 * `ksf_loyalty_transactions`) but read as `TB_PREF . "fa_customer_loyalty"`.
 * Neither name exists: the schema wrote
 * `fa_customer_loyalty` with no company prefix at all, and the code asked
 * for the legacy fa_-prefixed customer_loyalty table. Every read and write failed with
 * MySQL error 1146.
 *
 * The tables are module-owned (a loyalty record is not FA core data), so
 * they move into the 0_ksf_loyalty_* family.
 */
class LoyaltyTableNameTest extends TestCase
{
    private const BALANCES_TABLE = 'ksf_loyalty_balances';

    private const TRANSACTIONS_TABLE = 'ksf_loyalty_transactions';

    private string $moduleDir;

    protected function setUp(): void
    {
        $this->moduleDir = dirname(__DIR__, 2);
    }

    public function testLoyaltyDbDoesNotAddressTheNonExistentFaLoyaltyTables(): void
    {
        $contents = (string) file_get_contents($this->moduleDir . '/includes/loyalty_db.inc');

        $this->assertStringNotContainsString(
            'fa_customer_loyalty',
            $contents,
            'includes/loyalty_db.inc must not address the fa_customer_loyalty table: no schema creates it'
        );
    }

    public function testLoyaltyDbUsesTheModuleOwnedTables(): void
    {
        $contents = (string) file_get_contents($this->moduleDir . '/includes/loyalty_db.inc');

        $this->assertSame(
            6,
            substr_count($contents, 'TB_PREF . "' . self::BALANCES_TABLE),
            'every loyalty statement must read the module-owned balances table'
        );
    }

    public function testSchemaCreatesTheBalancesTable(): void
    {
        $contents = (string) file_get_contents($this->moduleDir . '/sql/ksf_loyalty_balances.sql');

        $this->assertStringContainsString(
            'CREATE TABLE IF NOT EXISTS `0_' . self::BALANCES_TABLE . '`',
            $contents
        );
    }

    public function testSchemaCreatesTheTransactionsTable(): void
    {
        $contents = (string) file_get_contents($this->moduleDir . '/sql/ksf_loyalty_transactions.sql');

        $this->assertStringContainsString(
            'CREATE TABLE IF NOT EXISTS `0_' . self::TRANSACTIONS_TABLE . '`',
            $contents
        );
    }

    public function testTransactionsForeignKeyTargetsTheBalancesTable(): void
    {
        $contents = (string) file_get_contents($this->moduleDir . '/sql/ksf_loyalty_transactions.sql');

        $this->assertStringContainsString(
            'REFERENCES `0_' . self::BALANCES_TABLE . '`',
            $contents,
            'the transactions table must reference the balances table that now exists'
        );
    }

    public function testActivationInstallsBothTables(): void
    {
        $contents = (string) file_get_contents($this->moduleDir . '/hooks.php');

        $this->assertStringContainsString(
            "'ksf_loyalty_balances.sql'     => array('ksf_loyalty_balances')",
            $contents,
            'activate_extension() must install the balances table'
        );
        $this->assertStringContainsString(
            "'ksf_loyalty_transactions.sql' => array('ksf_loyalty_transactions')",
            $contents,
            'activate_extension() must install the transactions table'
        );
    }
}
