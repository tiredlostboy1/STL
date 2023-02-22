#include "priority_queue.hpp"


void test_top() {
  PriorityQueue<int> mypq;
  mypq.push(5);
  mypq.push(25);
  mypq.push(15);

  assert(mypq.top() == 25);
}

void test_empty() {
  PriorityQueue<int> mypq;
  assert(mypq.empty());

  mypq.push(42);
  assert(!mypq.empty());
}

void test_empty_after_pop() {
  PriorityQueue<int> mypq;
  int sum{};

  for (int i = 1; i <= 10; i++)
    mypq.push(i);

  while (!mypq.empty()) {
    sum += mypq.top();
    mypq.pop();
  }

  assert(sum == 55);
}

void test_size() {

  PriorityQueue<int> myints;
  assert(myints.size() == 0);
  for (int i = 0; i < 5; i++) {
    myints.push(i);
    assert(myints.size() == i + 1);
  }
}

void test_swap() {
  PriorityQueue<int> mypq, mypq_for_swap;
  mypq.push(15);
  mypq.push(30);
  mypq.push(10);
  mypq_for_swap.push(101);
  mypq_for_swap.push(202);

  mypq.swap(mypq_for_swap);
  assert(mypq.size() == 2);
  assert(mypq_for_swap.size() == 3);
}

void test_pop() {
  PriorityQueue<int> mypq;

  mypq.push(30);
  mypq.push(100);
  mypq.push(25);
  mypq.push(40);

  while (!mypq.empty()) {
    int top = mypq.top();
    assert(top == mypq.pop());
  }
}

void test_push() {
  PriorityQueue<int> mypq;

  for (int i = 0; i < 5; ++i) {
    mypq.push(i);
    assert(mypq.top() == i);
  }
}

void test_operator_assignment_l_value() {
  PriorityQueue<int> mypq, mypq_copyed;
  mypq.push(1);
  mypq.push(2);
  mypq.push(10);
  mypq.push(5);

  mypq_copyed = mypq;

  assert(mypq.size() == mypq_copyed.size());
  assert(mypq.top() == mypq_copyed.top());
}

void test_operator_assignment_r_value() {
  PriorityQueue<int> mypq, mypq_copyed;
  mypq.push(1);
  mypq.push(2);
  mypq.push(10);
  mypq.push(5);

  mypq_copyed = std::move(mypq);

  assert(mypq.size() == 0);
  assert(mypq_copyed.size() == 4);
}

void test_copy_constructor() {
  PriorityQueue<int> mypq;
  mypq.push(1);
  mypq.push(2);
  mypq.push(10);
  mypq.push(5);

  PriorityQueue<int> mypq_copyed{mypq};

  assert(mypq.size() == mypq_copyed.size());
}

void test_move_constructor() {
  PriorityQueue<int> mypq;
  mypq.push(1);
  mypq.push(2);
  mypq.push(10);
  mypq.push(5);

  auto size_of_mypq = mypq.size();
  PriorityQueue<int> mypq_copyed{std::move(mypq)};

  assert(mypq_copyed.size() == size_of_mypq);
  assert(mypq.size() == 0);
}

void test_swap_with_empty_container() {
  PriorityQueue<int> mypq, mypq_for_swap;
  mypq.push(5);
  mypq.push(25);
  mypq.push(15);

  mypq.swap(mypq_for_swap);

  assert(mypq.size() == 0);
  assert(mypq_for_swap.size() == 3);
  assert(mypq_for_swap.top() == 25);
}

int main() {

  test_top();
  test_empty();
  test_empty_after_pop();
  test_push();
  test_pop();
  test_size();
  test_swap();

  test_operator_assignment_l_value();
  test_operator_assignment_r_value();

  test_copy_constructor();
  test_move_constructor();

  test_swap_with_empty_container();

  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  PriorityQueue<int> p(v);
  // for(int i=0; i<9;++i){
  //   std::cout<<p.data[i]<< " ";
  // }
}