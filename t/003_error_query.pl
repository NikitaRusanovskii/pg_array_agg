
use strict;
use warnings FATAL => 'all';

use PostgreSQL::Test::Cluster;
use Test::More;

my $node = PostgreSQL::Test::Cluster->new('error_query');

$node->init;
$node->start;

$node->safe_psql('postgres', 'CREATE EXTENSION pg_array_agg');

sub check_error
{
    my ($query, $error_pattern, $description) = @_;

    my $stderr = '';

    my $result = $node->psql(
        'postgres',
        $query,
        stderr => \$stderr
    );

    isnt(
        $result,
        0,
        "$description: query fails"
    );

    like(
        $stderr,
        $error_pattern,
        "$description: expected error message"
    );

    my $valid_result = $node->safe_psql(
        'postgres',
        'SELECT array_sum(ARRAY[1, 2, 3]::integer[]);'
    );

    is(
        $valid_result,
        '6',
        "$description: server executes valid query after error"
    );
};

check_error(
    q{
        SELECT array_sum(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[]
        );
    },
    qr/Invalid ndim of array: 3/i,
    'array_sum rejects ndim=3'
);
check_error(
    q{
        SELECT array_product(ARRAY['c', 'b', 'a']::text[]);
    },
    qr/Invalid elemtype/i,
    'array_product rejects text elements'
);

check_error(
    q{
        SELECT array_avg(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[]
        );
    },
    qr/Invalid ndim of array: 3/i,
    'array_avg rejects ndim=3'
);
check_error(
    q{
        SELECT array_min(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[]
        );
    },
    qr/Invalid ndim of array: 3/i,
    'array_min rejects ndim=3'
);

check_error(
    q{
        SELECT array_max(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[]
        );
    },
    qr/Invalid ndim of array: 3/i,
    'array_max rejects ndim=3'
);

check_error(
    q{
        SELECT array_count(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[],
            1
        );
    },
    qr/Invalid ndim of array: 3/i,
    'array_count rejects ndim=3'
);

check_error(
    q{
        SELECT array_count_if(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[],
            'x > 0'
        );
    },
    qr/more than two dimensions|multidimensional|dimension/i,
    'array_count_if rejects ndim=3'
);
check_error(
    q{
        SELECT array_all(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[],
            'x < 10'
        );
    },
    qr/more than two dimensions|multidimensional|dimension/i,
    'array_all rejects ndim=3'
);

check_error(
    q{
        SELECT array_any(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[],
            'x == 3'
        );
    },
    qr/more than two dimensions|multidimensional|dimension/i,
    'array_any rejects ndim=3'
);

check_error(
    q{
        SELECT array_none(
            ARRAY[[[1, 2]], [[3, 4]]]::integer[],
            'x == 7'
        );
    },
    qr/more than two dimensions|multidimensional|dimension/i,
    'array_none rejects ndim=3'
);

$node->stop;

done_testing();
