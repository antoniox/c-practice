typedef struct ListNode ListNode;

struct ListNode {
    void * value;
    ListNode * next;
    ListNode * prev;
};


typedef struct Deque Deque;

struct Deque {
    ListNode * begin;
    ListNode * end;
    void (*destroy_node)(ListNode *);
};


void deque_init(Deque * deque, void (*destroy_node)(ListNode *));
void deque_destroy(Deque * deque);

void deque_pushfront(Deque * deque, void * value);
void * deque_front(Deque * deque);
void deque_popfront(Deque * deque);

void deque_pushback(Deque * deque, void * value);
void * deque_back(Deque * deque);
void deque_popback(Deque * deque);

void default_destroy_list_node(ListNode *);
