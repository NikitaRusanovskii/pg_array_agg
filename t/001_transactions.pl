use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('transaction_test');
$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_agg');
my $result = $node->safe_psql('postgres', 
    q{
        BEGIN;
        SELECT array_sum(ARRAY[1,2,3,4,5]::Integer[]);
        SELECT array_product(ARRAY[1,2,3,4,5]::Integer[]);
        COMMIT;
    });

like (
    $result,
    qr/\b15\b.*\b120\b/s,
    'test array_sum & array_product inside transaction'
);

$result = $node->safe_psql('postgres', 
    q{
        SELECT array_sum(ARRAY[1,2,3,4,5]::Integer[]);
        BEGIN;
        SELECT array_sum(ARRAY[1,2,3,4,5]::Integer[]);
        COMMIT;
        ROLLBACK;
        SELECT array_sum(ARRAY[1,2,3,4,5]::Integer[]);
    });

like (
    $result,
    qr/\b15\b.*\b15\b.*\b15\b/s,
    'test array_sum before transaction, inside transaction & after rollback'
);

$node->stop;
done_testing();
