#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX_STRING_LENGTH 6

struct package
{
	char* id;
	int weight;
};

typedef struct package package;

struct post_office
{
	int min_weight;
	int max_weight;
	package* packages;
	int packages_count;
};

typedef struct post_office post_office;

struct town
{
	char* name;
	post_office* offices;
	int offices_count;
};

typedef struct town town;



void print_all_packages(town t) {
    (void)t;
}

void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) {	
    assert(source != NULL && target != NULL);
    assert(source->name != NULL && target->name != NULL);
    // get post offices. 
    post_office *src_po = source->offices+source_office_index;
    post_office *dst_po = target->offices+target_office_index;

    int r_count = 0;
    package rejected[1000];
    int a_count = 0;
    package accepted[1000];
    
    package *pkg = 0;
    for (int i=0; i<src_po->packages_count; i++) {
        pkg = src_po->packages+i;
        printf("sending pkg: %s %d", pkg->id, pkg->weight);
        if (pkg->weight < dst_po->min_weight || pkg->weight > dst_po->max_weight) {
            printf("pkg rejected based on weight limits: %s %d", pkg->id, pkg->weight);
            // put package in rejected queue.
            memcpy(&rejected[r_count++], pkg, sizeof(package));
        } else {
            memcpy(&accepted[a_count++], pkg, sizeof(package));
        }
        free(pkg);
    }
}

town town_with_most_packages(town* towns, int towns_count) {
    (void)towns;
    (void)towns_count;
    town t = {};
    return t;
}

town* find_town(town* towns, int towns_count, char* name) {
    for (int i=0; i<towns_count; i++) {
        if (!strcmp(towns[i].name, name)) {
            return towns+i;
        }
    }
    return 0;
}

typedef struct {
    int towns_count;
    town* towns;
    int queries;
    int* cases;
}test_table_t;

int main()
{
    //package *pa_0 = (package*)calloc(0, sizeof(package*)*2)
    package pa_0[2] = {
        {
            .id = "a",
            .weight = 2,
        },
        {
            .id = "b",
            .weight = 3,
        },
    };

    package pa_1[2] = {
        {
            .id = "c",
            .weight = 2,
        },
    };

    package pb[4] = {
        {
            .id = "d",
            .weight = 1,
        },
        {
            .id = "e",
            .weight = 2,
        },
        {
            .id = "f",
            .weight = 3,
        },
        {
            .id = "h",
            .weight = 4,
        },
    };

    post_office pos_a[2] = {
        {
            .min_weight = 1,
            .max_weight = 3,
            .packages = pa_0,
            .packages_count = 2,
        },
        {
            .min_weight = 2,
            .max_weight = 4,
            .packages = pa_1,
            .packages_count = 1,
        }
    };

    post_office pos_b[2] = {
        {
            .min_weight = 1,
            .max_weight = 4,
            .packages = pb,
            .packages_count = 4,
        }
    };
    town towns[2] = {
        {
            .name = "A",
            .offices = pos_a,
            .offices_count = 2,
        },
        {
            .name = "B",
            .offices = pos_b,
            .offices_count = 1,
        },
    };

    int cs[5] = { 3, 2, 3, 1, 1 };

    test_table_t tt = {
        .towns_count = 2,
        .towns = towns,
        .queries = 5,
        .cases = cs,
    };

    char town_name[1000] = {};
	for (int q=0; q<tt.queries; q++) {
		switch (tt.cases[q]) {
		case 1:
			scanf("%s", town_name);
			town* t = find_town(tt.towns, tt.towns_count, town_name);
			print_all_packages(*t);
			break;
		case 2:
			scanf("%s", town_name);
			town* source = find_town(tt.towns, tt.towns_count, town_name);
			int source_index;
			scanf("%d", &source_index);
			scanf("%s", town_name);
			town* target = find_town(tt.towns, tt.towns_count, town_name);
			int target_index;
			scanf("%d", &target_index);
			send_all_acceptable_packages(source, source_index, target, target_index);
			break;
		case 3:
			printf("Town with the most number of packages is %s\n", town_with_most_packages(tt.towns, tt.towns_count).name);
			break;
		}
    }
	
	return 0;
}
