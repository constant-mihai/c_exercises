#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX_STRING_LENGTH 6

#define LOG_ENABLE 1

#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) do { if (LOG_ENABLE == 1) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__); } while (0)


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
    printf("%s\n", t.name);
    for (int po=0; po<t.offices_count; po++) {
        printf("\t%d:\n", po);
        for (int p=0; p<t.offices[po].packages_count; p++) {
            printf("\t\t%s\n", t.offices[po].packages[p].id);
        }
    }
}

void send_all_acceptable_packages(town* source, int source_office_index, town* target, int target_office_index) {	
    assert(source != NULL && target != NULL);
    assert(source->name != NULL && target->name != NULL);
    // get post offices. 
    post_office *src_po = source->offices+source_office_index;
    post_office *dst_po = target->offices+target_office_index;

    int r_count = 0;
    package rejected[1000] = {};
    int a_count = 0;
    package accepted[1000] = {};
    
    package *pkg = 0;
    for (int i=0; i<src_po->packages_count; i++) {
        pkg = src_po->packages+i;
        LOG("sending pkg: %s %d", pkg->id, pkg->weight);
        if (pkg->weight < dst_po->min_weight || pkg->weight > dst_po->max_weight) {
            LOG("pkg rejected based on weight limits: %s %d", pkg->id, pkg->weight);
            // put package in rejected queue.
            memcpy(&rejected[r_count++], pkg, sizeof(package));
        } else {
            memcpy(&accepted[a_count++], pkg, sizeof(package));
        }
    }
    free(src_po->packages);

    // send the rejected packets back
    // TODO, need to reallocate and copy memory over.
    src_po->packages = (package*) malloc(sizeof(package)*r_count);
    src_po->packages_count = r_count;
    for (int i=0; i<r_count; i++) {
        memcpy(&src_po->packages[i], &rejected[i], sizeof(package));
    }
    
    // enque the accepted packages
    size_t merged_package_queue_size = dst_po->packages_count + a_count;
    package *merged_package_queue = (package*) malloc(sizeof(package)*merged_package_queue_size);
    // move over old packages
    int merge_idx = 0;
    for (; merge_idx<dst_po->packages_count; merge_idx++) {
        memcpy(&merged_package_queue[merge_idx], dst_po->packages+merge_idx, sizeof(package));
    }
    free(dst_po->packages);
    // move over new packages
    int j = 0;
    for (size_t i=merge_idx; i<merged_package_queue_size; i++, j++) {
        memcpy(&merged_package_queue[i], &accepted[j], sizeof(package));
    }
    dst_po->packages = merged_package_queue;
    dst_po->packages_count = merged_package_queue_size;
}

town town_with_most_packages(town* towns, int towns_count) {
    int *sum_packages = malloc(sizeof(int)*towns_count);
    for (int i=0; i<towns_count; i++) {
        for (int po=0; po<towns[i].offices_count; po++) {
            sum_packages[i] += towns[i].offices[po].packages_count;
        }
    }

    int max = 0;
    int max_idx = 0;
    for (int i=0; i<towns_count; i++) {
        if (sum_packages[i] > max) {
            max = sum_packages[i];
            max_idx = i;
        }
    }
    return towns[max_idx];
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
    package *pa_0 = (package*)malloc(sizeof(package)*2);
    pa_0[0].id = "a";
    pa_0[0].weight = 2;
    pa_0[1].id = "b";
    pa_0[1].weight = 3;

    package *pa_1 = (package*)malloc(sizeof(package)*1);
    pa_1->id = "c";
    pa_1->weight = 2;

    package *pb = (package*)malloc(sizeof(package)*4);
    pb[0].id = "d";
    pb[0].weight = 1;
    pb[1].id = "e";
    pb[1].weight = 2;
    pb[2].id = "f";
    pb[2].weight = 3;
    pb[3].id = "h";
    pb[3].weight = 4;

    post_office *pos_a = (post_office*)malloc(sizeof(post_office)*2);
    pos_a[0].min_weight = 1;
    pos_a[0].max_weight = 3;
    pos_a[0].packages = pa_0;
    pos_a[0].packages_count = 2;
    pos_a[1].min_weight = 2;
    pos_a[1].max_weight = 4;
    pos_a[1].packages = pa_1;
    pos_a[1].packages_count = 1;

    post_office *pos_b = (post_office*)malloc(sizeof(post_office)*2);
    pos_b->min_weight = 1;
    pos_b->max_weight = 4;
    pos_b->packages = pb;
    pos_b->packages_count = 4;

    town *towns = (town*)malloc(sizeof(town)*2);
    towns[0].name = "A";
    towns[0].offices = pos_a;
    towns[0].offices_count = 2;
    towns[1].name = "B";
    towns[1].offices = pos_b;
    towns[1].offices_count = 1;

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
			LOG("Town with the most number of packages is %s\n", town_with_most_packages(tt.towns, tt.towns_count).name);
			break;
		}
    }
	
	return 0;
}
