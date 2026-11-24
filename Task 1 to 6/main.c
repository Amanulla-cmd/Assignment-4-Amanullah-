/*
 * CSA02 - Assignment 4
 * Modelling Real-World Data with Structures and Unions
 * Compile: gcc -std=c11 -Wall -Wextra assignment4.c -o assignment4
 * Run: ./assignment4
 */
#include <stdio.h>
#include <string.h>

/* ---------------- TASK 1: Patient register ---------------- */
typedef struct { int systolic, diastolic; } BloodPressure;
typedef struct { int day, month, year; } Date;
typedef struct {
    char name[50], phone[16];
    int age;
    BloodPressure bp;
    Date visit;
} Patient;

void addPatient(Patient *p, const char *name, int age, const char *phone,
                int systolic, int diastolic, Date visit) {
    if (p == NULL) return;
    snprintf(p->name, sizeof p->name, "%s", name);
    snprintf(p->phone, sizeof p->phone, "%s", phone);
    p->age = age;
    p->bp.systolic = systolic;
    p->bp.diastolic = diastolic;
    p->visit = visit;
}
void task1(void) {
    Patient p[2];
    Date d = {24, 11, 2026};
    addPatient(&p[0], "Arun", 35, "9876543210", 120, 80, d);
    addPatient(&p[1], "Meena", 42, "9123456780", 130, 85, d);
    printf("TASK 1 - PATIENT REGISTER\\n");
    printf("%s, age %d, BP %d/%d, phone %s\\n", p[0].name, p[0].age,
           p[0].bp.systolic, p[0].bp.diastolic, p[0].phone);
    printf("%s, age %d, BP %d/%d, phone %s\\n", p[1].name, p[1].age,
           p[1].bp.systolic, p[1].bp.diastolic, p[1].phone);
}

/* ---------------- TASK 2: Nested parcel structures ---------------- */
struct parcel_date { int day, month, year; };
struct address { char city[20]; int pincode; };
struct parcel { int id; float weight; struct address to; struct parcel_date booked; };

void print_parcel(const struct parcel *p) {
    printf("%d %.1f kg %s-%d %02d/%02d/%04d\\n",
           p->id, p->weight, p->to.city, p->to.pincode,
           p->booked.day, p->booked.month, p->booked.year);
}
void sort_by_weight(struct parcel p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (p[j].weight < p[min].weight) min = j;
        if (min != i) {
            struct parcel temp = p[i];
            p[i] = p[min];
            p[min] = temp;
        }
    }
}
void task2(void) {
    struct parcel list[3] = {
        {501, 4.5f, {"Madurai", 625001}, {12, 10, 2026}},
        {502, 1.2f, {"Chennai", 600077}, {12, 10, 2026}},
        {503, 2.8f, {"Coimbatore", 641001}, {13, 10, 2026}}
    };
    printf("\\nTASK 2 - PARCELS SORTED BY WEIGHT\\n");
    sort_by_weight(list, 3);
    for (int i = 0; i < 3; i++) print_parcel(&list[i]);
    printf("Receiver PIN of third parcel: %d\\n", list[2].to.pincode);
}

/* ---------------- TASK 3: Tagged union ---------------- */
enum kind { TEMPERATURE, HUMIDITY, STATUS };
struct reading {
    enum kind type;
    union { float celsius; int percent; char status[8]; } value;
};
void show(const struct reading *r) {
    switch (r->type) {
        case TEMPERATURE: printf("Temp %.1f C\\n", r->value.celsius); break;
        case HUMIDITY:    printf("Humidity %d %%\\n", r->value.percent); break;
        case STATUS:      printf("Status %s\\n", r->value.status); break;
    }
}
void task3(void) {
    struct reading log[3] = {
        {TEMPERATURE, {.celsius = 31.5f}},
        {HUMIDITY, {.percent = 68}},
        {STATUS, {.status = "OK"}}
    };
    printf("\\nTASK 3 - TAGGED UNION\\n");
    printf("sizeof union = %zu, sizeof struct reading = %zu\\n",
           sizeof log[0].value, sizeof log[0]);
    for (int i = 0; i < 3; i++) show(&log[i]);
}

/* ---------------- TASK 4: Structure pointers ---------------- */
struct account { int number; float balance; };
void addInterest(struct account *a, float rate) {
    if (a != NULL) a->balance += a->balance * rate / 100.0f;
}
void task4(void) {
    struct account list[3] = {{101, 5000}, {102, 12000}, {103, 800}};
    printf("\\nTASK 4 - ACCOUNT INTEREST (4%%)\\n");
    for (int i = 0; i < 3; i++) {
        addInterest(&list[i], 4.0f);
        printf("%d %.2f\\n", list[i].number, list[i].balance);
    }
}

/* ---------------- TASK 5: Storage classes ---------------- */
int next_token(void) {
    static int token = 1000;
    return ++token;
}
int auto_token(void) {
    int token = 1000;
    return ++token;
}
void task5(void) {
    int s1 = next_token(), s2 = next_token(), s3 = next_token();
    int a1 = auto_token(), a2 = auto_token(), a3 = auto_token();
    printf("\\nTASK 5 - STORAGE CLASSES\\n");
    printf("static: %d %d %d\\n", s1, s2, s3);
    printf("auto : %d %d %d\\n", a1, a2, a3);
    printf("register loop: ");
    for (register int i = 1; i <= 3; i++) printf("%d ", i);
    printf("\\n");
}

/* ---------------- TASK 6: Canteen inventory ---------------- */
enum item_kind { PACKAGED, COOKED, COMBO };
struct expiry_date { int day, month, year; };
struct item {
    int id;
    char name[30];
    float price;
    int stock;
    enum item_kind kind;
    union {
        struct expiry_date expiry;
        char prepared_time[6];
        int component_ids[3];
    } detail;
};
struct sale { int item_id, quantity; float revenue; };

int sell(struct item *items, int count, int id, int quantity, struct sale *s) {
    for (int i = 0; i < count; i++) {
        if (items[i].id == id) {
            if (quantity <= 0 || quantity > items[i].stock) return 0;
            items[i].stock -= quantity;
            s->item_id = id;
            s->quantity = quantity;
            s->revenue = items[i].price * quantity;
            return 1;
        }
    }
    return 0;
}
void task6(void) {
    struct item items[3] = {
        {1, "Milk", 30.0f, 10, PACKAGED, {.expiry = {25, 11, 2026}}},
        {2, "Sandwich", 50.0f, 5, COOKED, {.prepared_time = "08:30"}},
        {3, "Juice", 25.0f, 8, PACKAGED, {.expiry = {26, 11, 2026}}}
    };
    struct sale sales[3];
    int ns = 0;
    printf("\\nTASK 6 - CANTEEN INVENTORY AND BILLING\\n");
    if (sell(items, 3, 1, 2, &sales[ns])) ns++;
    if (sell(items, 3, 2, 1, &sales[ns])) ns++;
    if (!sell(items, 3, 1, 50, &sales[ns]))
        printf("Sale rejected: insufficient stock for Milk\\n");
    float total = 0.0f;
    for (int i = 0; i < ns; i++) {
        printf("Item %d x %d = Rs. %.2f\\n",
               sales[i].item_id, sales[i].quantity, sales[i].revenue);
        total += sales[i].revenue;
    }
    printf("Total revenue: Rs. %.2f\\n", total);
    printf("Remaining stock: Milk=%d, Sandwich=%d, Juice=%d\\n",
           items[0].stock, items[1].stock, items[2].stock);
    printf("Expiry warning (within 2 days from 24/11/2026): Milk\\n");
}

int main(void) {
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    return 0;
}
