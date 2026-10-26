use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('transaction_test');
$node->init;
$node->start;

sub check_in_transaction {
    my ($query, $pattern, $description) = @_;
    my $result = $node->safe_psql('postgres', $query);
    like($result, $pattern, $description);
}

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_agg');

check_in_transaction(
    q{
        BEGIN;
        SELECT array_sum(ARRAY[1,2,3,4,5]::integer[]);
        SELECT array_product(ARRAY[1,2,3,4,5]::integer[]);
        COMMIT;
    },
    qr/\b15\b.*\b120\b/s,
    'test array_sum & array_product inside transaction'
);

check_in_transaction(
    q{
        SELECT array_sum(ARRAY[1,2,3,4,5]::integer[]);
        BEGIN;
        SELECT array_sum(ARRAY[1,2,3,4,5]::integer[]);
        ROLLBACK;
        SELECT array_sum(ARRAY[1,2,3,4,5]::integer[]);
    },
    qr/\b15\b.*\b15\b.*\b15\b/s,
    'test array_sum before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_avg(ARRAY[1,2,3,4,5]::integer[]);
        COMMIT;
    },
    qr/\b3\.0000000000000000\b/s,
    'test array_avg inside transaction'
);

check_in_transaction(
    q{
        SELECT array_avg(ARRAY[1,2,3,4,5]::integer[]);
        BEGIN;
        SELECT array_avg(ARRAY[1,2,3,4,5]::integer[]);
        ROLLBACK;
        SELECT array_avg(ARRAY[1,2,3,4,5]::integer[]);
    },
    qr/\b3\.0000000000000000\b.*\b3\.0000000000000000\b.*\b3\.0000000000000000\b/s,
    'test array_avg before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_count(ARRAY[1,2,3,4,5]::integer[], 3);
        COMMIT;
    },
    qr/\b1\b/s,
    'test array_count inside transaction'
);

check_in_transaction(
    q{
        SELECT array_count(ARRAY[1,2,3,4,5]::integer[], 3);
        BEGIN;
        SELECT array_count(ARRAY[1,2,3,4,5]::integer[], 3);
        ROLLBACK;
        SELECT array_count(ARRAY[1,2,3,4,5]::integer[], 3);
    },
    qr/\b1\b.*\b1\b.*\b1\b/s,
    'test array_count before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_count_if(ARRAY[1,2,3,4,5]::integer[], 'x % 2 = 0');
        COMMIT;
    },
    qr/\b2\b/s,
    'test array_count_if inside transaction'
);

check_in_transaction(
    q{
        SELECT array_count_if(ARRAY[1,2,3,4,5]::integer[], 'x % 2 = 0');
        BEGIN;
        SELECT array_count_if(ARRAY[1,2,3,4,5]::integer[], 'x % 2 = 0');
        ROLLBACK;
        SELECT array_count_if(ARRAY[1,2,3,4,5]::integer[], 'x % 2 = 0');
    },
    qr/\b2\b.*\b2\b.*\b2\b/s,
    'test array_count_if before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_min(ARRAY[1,2,3,4,5]::integer[]);
        COMMIT;
    },
    qr/\b1\b/s,
    'test array_min inside transaction'
);

check_in_transaction(
    q{
        SELECT array_min(ARRAY[1,2,3,4,5]::integer[]);
        BEGIN;
        SELECT array_min(ARRAY[1,2,3,4,5]::integer[]);
        ROLLBACK;
        SELECT array_min(ARRAY[1,2,3,4,5]::integer[]);
    },
    qr/\b1\b.*\b1\b.*\b1\b/s,
    'test array_min before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_max(ARRAY[1,2,3,4,5]::integer[]);
        COMMIT;
    },
    qr/\b5\b/s,
    'test array_max inside transaction'
);

check_in_transaction(
    q{
        SELECT array_max(ARRAY[1,2,3,4,5]::integer[]);
        BEGIN;
        SELECT array_max(ARRAY[1,2,3,4,5]::integer[]);
        ROLLBACK;
        SELECT array_max(ARRAY[1,2,3,4,5]::integer[]);
    },
    qr/\b5\b.*\b5\b.*\b5\b/s,
    'test array_max before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_all(ARRAY[1,2,3,4,5]::integer[], 'x > 0');
        COMMIT;
    },
    qr/\bt\b/s,
    'test array_all inside transaction'
);

check_in_transaction(
    q{
        SELECT array_all(ARRAY[1,2,3,4,5]::integer[], 'x > 0');
        BEGIN;
        SELECT array_all(ARRAY[1,2,3,4,5]::integer[], 'x > 0');
        ROLLBACK;
        SELECT array_all(ARRAY[1,2,3,4,5]::integer[], 'x > 0');
    },
    qr/\bt\b.*\bt\b.*\bt\b/s,
    'test array_all before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_any(ARRAY[1,2,3,4,5]::integer[], 'x == 3');
        COMMIT;
    },
    qr/\bt\b/s,
    'test array_any inside transaction'
);

check_in_transaction(
    q{
        SELECT array_any(ARRAY[1,2,3,4,5]::integer[], 'x == 3');
        BEGIN;
        SELECT array_any(ARRAY[1,2,3,4,5]::integer[], 'x == 3');
        ROLLBACK;
        SELECT array_any(ARRAY[1,2,3,4,5]::integer[], 'x == 3');
    },
    qr/\bt\b.*\bt\b.*\bt\b/s,
    'test array_any before transaction, inside transaction & after rollback'
);

check_in_transaction(
    q{
        BEGIN;
        SELECT array_none(ARRAY[1,2,3,4,5]::integer[], 'x == 7');
        COMMIT;
    },
    qr/\bt\b/s,
    'test array_none inside transaction'
);

check_in_transaction(
    q{
        SELECT array_none(ARRAY[1,2,3,4,5]::integer[], 'x == 7');
        BEGIN;
        SELECT array_none(ARRAY[1,2,3,4,5]::integer[], 'x == 7');
        ROLLBACK;
        SELECT array_none(ARRAY[1,2,3,4,5]::integer[], 'x == 7');
    },
    qr/\bt\b.*\bt\b.*\bt\b/s,
    'test array_none before transaction, inside transaction & after rollback'
);

$node->stop;
done_testing();