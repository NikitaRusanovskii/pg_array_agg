
use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('error_recovery');

$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_agg');

my $error = eval {
    $node->safe_psql(
        'postgres',
        q{
            SELECT array_sum(
                ARRAY[[1, 2], [3, 4]]::integer[]
            );
        }
    );
    '';
};

like(
    $@,
    qr/multidimensional|dimension|array/i,
    'array_sum rejects a multidimensional array'
);

my $result = $node->safe_psql(
    'postgres',
    'SELECT array_sum(ARRAY[1, 2, 3]::integer[]);'
);

is(
    $result,
    '6',
    'server executes valid queries after an error'
);

$node->stop;

done_testing();
