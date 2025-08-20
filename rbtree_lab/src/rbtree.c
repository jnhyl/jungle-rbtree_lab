#include "rbtree.h"

#include <stdlib.h>


// -------------------------------------------------------------------------------------------------------------

// RBTree 시작, 루트 노드 및 nil 노드를 가리키는 구조체 포인터 생성
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed

  // nil 노드를 생성해줘야 한다. 
  // RBTree에서 모든 정의되지 않은 노드 위치에는 NULL 대신 nil 노드가 존재해 실제 노드처럼 처리된다.
  // 생성한 nil 노드는 부모나 자식 노드가 없는 모든 노드들이 같이 사용
  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;   // nil 노드는 항상 검은색
  p->nil->parent = p->nil->left = p->nil->right = p->nil; // nil 노드의 모든 부모, 자식 노드들은 본인을 향하도록

  p->root = p->nil;   // root 노드 또한 정의되지 않았기 때문에 root 포인터는 nil 노드를 향한다.
  return p;
}

// ------------------------------------------------------------------------------------------------------------

// 트리 삭제 (트리 및 모든 노드 메모리 해제)
// 후위 순회로 nil 노드를 제외한 모든 노드 삭제
void removeAllNode(rbtree *t, node_t *root) {
  if (!root || root == t->nil) return;  // NULL Guard & 빈 트리일 경우(기저 조건)

  removeAllNode(t, root->left);
  removeAllNode(t, root->right);
  free(root);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t == NULL) return;      // NULL Guard

  removeAllNode(t, t->root);  // (nil 노드를 제외한) 트리의 모든 노드 제거
  free(t->nil);               // nil 노드 제거
  free(t);                    // 트리 제거
}

// 논리적으로는 맞으나 실무적으로는 수정해야 할 부분
// 참고 : https://chatgpt.com/s/t_689f082f01b88191b8c9fe5387c58ea0
// 1. 트리가 nonsentinel일 경우 고려 2. 노드 내부 필드의 변수에 동적 할당이 되어 있을 경우 3. 재귀로 인한 스택 오버플로우 방지

// -------------------------------------------------------------------------------------------------------------
// insert 함수 구현 전체 코드
// 참고 : https://chatgpt.com/s/t_68a09fe3f6348191897951b66c216a11


// 왼쪽 회전
// clrs와 사소하게 다른 버전 (내가 짠 거에 논리적 오류 부분은 챗 지피티의 도움을 받아 고침)
// 개선 사항 1. sentinel 오염 여부 판단 필요 2. 회전 시 논리적 순서가 불명확 3. 변수명 혼동 가능성
// 참고 : https://chatgpt.com/s/t_68a025d8a9248191a03ff332a484e91f

// void lrotate(rbtree *t, node_t *root) {
//   if (t == NULL || root == NULL) return;

//   // 회전 시키려는 기준 노드가 비어있거나(=sentinel) 오른쪽 자식이 존재하지 않으면 아무것도 하지 말고 종료
//   if (root == t->nil || root->right == t->nil) return;
  
//   node_t *parent = root->parent;    // 루트 노드의 부모 노드
//   node_t *rchild = root->right;     // 루트 노드의 오른쪽 자식

//   // 루트 노드의 오른쪽 자식 노드를 부모 노드와 연결
//   if (parent == t->nil) {   // RBTree의 루트 노드였을 경우
//     t->root = rchild;
//   } else if (root == parent->left) {  // 서브 트리의 루트 노드가 부모의 왼쪽 자식이였을 경우
//     parent->left = rchild;
//   } else {                            // 서브 트리의 루트 노드가 부모의 오른쪽 자식이였을 경우
//     parent->right = rchild;
//   }
//   rchild->parent = parent;            // 부모 노드에 연결(루트 노드 자리일 경우 nil 노드에 연결)

//   // 회전한 루트 노드 처리

//   // 루트 노드의 오른쪽 자식을 rchild의 왼쪽 자식으로 채움
//   root->right = rchild->left;
//   if (rchild->left != t->nil) {  // rchild의 왼쪽 자식이 존재하면(= nil 노드가 아니면)
//     rchild->left->parent = root;
//   }

//   // 회전 후 루트 노드는 rchild의 왼쪽 자식으로 위치
//   rchild->left = root;
//   root->parent = rchild;
// }

// 왼쪽 회전, CLRS 버전
void lrotate(rbtree *t, node_t *x) {
  if (!t || !x) return; // NULL Guard

  // x와 x의 오른쪽 자식 노드가 정의되어 있지 않으면 종료
  if (x == t->nil || x->right == t->nil) return;

  // x 노드의 오른쪽 자식을 y라고 명명
  node_t *y = x->right;

  // 회전 전 y의 왼쪽 자식 노드들을 x의 오른쪽 자식 노드로 옮긴다.
  x->right = y->left;
  if (y->left != t->nil) {  // y의 왼쪽 자식이 존재한다면 부모를 x로 정의해준다.
    y->left->parent = x;
  }

  // y노드를 기존 x의 부모 노드와 연결
  y->parent = x->parent;  // y의 부모는 기존 x의 부모(x가 루트 노드일 경우 x.perent == t.nil)
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x->parent->left == x) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  // x와 y관계 재정립, 회전 끝!
  y->left = x;
  x->parent = y;
}

// 오른쪽 회전, CLRS 버전
void rrotate(rbtree *t, node_t *x) {    // x : 회전의 기준이 되는 노드
  if (!t || !x) return;
  if (x == t->nil || x->left == t->nil) return;  // 기준 노드가 빈 노드거나(= sentinel) 왼쪽 자식 노드가 없으면 종료

  node_t *y = x->left;   // y : 기준 노드(x)의 왼쪽 자식 노드

  // y의 오른쪽 자식 노드를 x의 왼쪽 자식에 위치 시킨다.
  x->left = y->right;
  if (y->right != t->nil) {  // y의 오른쪽 자식 노드가 존재할 경우(!= nil node)
    y->right->parent = x;
  }

  // 부모 노드 필드에 y 노드 추가
  y->parent = x->parent;  // 오른쪽 회전, y가 x의 부모 노드에 연결된다.
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  // 오른쪽 회전 후 x는 y의 오른쪽 자식이 된다.
  y->right = x;
  x->parent = y;
}

// 참고 : https://chatgpt.com/s/t_68a09f0fade88191af9f67d1b2f8be4a
void insertFixUp(rbtree *t, node_t *z) {
  if (!t || !z) return;

  // #4. 레드의 자식은 블랙 (z의 부모 노드는 black이여야 한다.)
  while (z->parent->color == RBTREE_RED){   // z의 부모 노드가 red일 때 fixup을 반복해라
    // 부모 노드가 조부모 노드의 왼쪽에 존재할 때
    if (z->parent == z->parent->parent->left){
      // y는 z의 삼촌 노드, 부모 노드의 형제 노드
      node_t *y = z->parent->parent->right;
      
      // case 1
      // 조부모 노드는 black이고 부모 노드와 삼촌 노드가 red일 때
      if (y->color == RBTREE_RED) {
        z->parent->parent->color = RBTREE_RED;      // 조부모 노드의 색과
        z->parent->color = y->color = RBTREE_BLACK; // 두 형제 노드의 색을 서로 바꾼다. (#5 만족)
        z = z->parent->parent;  // 조부모 노드가 red가 됐으므로 조부모 노드를 기준으로 #2, #4 원칙에 위배되진 않는지 확인
      } else {
        // case2, 이 경우 굽힌 부분을 펴주고 바로 case3으로 진행된다.
        if (z == z->parent->right) {
          // 회전 전에 z의 부모를 z로 갱신해주는게 좋다. 회전 후에는 노드간 부모 - 자식 간 관계가 뒤바껴서 이후에 갱신하다가는 꼬인다.
          z = z->parent;    // z의 부모 노드를 기준으로 회전 후 case3으로 접근
          lrotate(t, z);
        }
        // case3
        z->parent->parent->color = RBTREE_RED;  // 조부모 노드를 red로 하고
        z->parent->color = RBTREE_BLACK;        // 부모 노드를 black으로 해서
        rrotate(t, z->parent->parent);          // 오른쪽으로 회전하면 bst 특성을 지키면서 red 노드를 옆으로 옮길 수 있다.
      }
    }

    // 부모 노드가 조부모 노드의 오른쪽에 존재할 때
    // 위와 좌우반전 모드
    else {
      // y = 삼촌 노드
      node_t *y = z->parent->parent->left;

      // case1
      if (y->color == RBTREE_RED) {
        z->parent->parent->color = RBTREE_RED;
        z->parent->color = y->color = RBTREE_BLACK;
        z = z->parent->parent;
      } else {
        // case2
        if (z == z->parent->left) {
          z = z->parent;
          rrotate(t, z);
        }

        // case3
        z->parent->parent->color = RBTREE_RED;
        z->parent->color = RBTREE_BLACK;
        lrotate(t, z->parent->parent);
      }
    }
  }
  // #2. 루트는 블랙
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  if (t == NULL) return NULL; // NULL Guard

  // 삽입 장소 탐색 (BST, 대신 sentinel 버전)
  node_t *cur = t->root;
  node_t *parent = t->nil;

  while (cur != t->nil) {
    parent = cur;
    if (key < cur->key){
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  // 삽입할 노드 생성
  node_t *z = malloc(sizeof(node_t));
  if (!z) return NULL;  // 메모리 할당 실패

  z->color = RBTREE_RED;
  z->key = key;
  z->left = z->right = t->nil;
  
  z->parent = parent;
  if (z->parent == t->nil) {    // 만일 삽입 위치가 루트 노드일 떄(첫 생성일 때)
    t->root = z;
  } else {
    if (z->key < parent->key) {
      parent->left = z;
    } else {
      parent->right = z;
    }
  }

  insertFixUp(t, z);

  return z;
}

// ---------------------------------------------------------------------------------------------------------------

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (!t) return NULL; // 널 가드

  node_t *cur = t->root;

  while (cur != t->nil) {
    if (key < cur->key) {
      cur = cur->left;
    } else if (key > cur->key) {
      cur = cur->right;
    } else {
      return cur;   // key 값을 가진 노드 주소 반환
    }
  }

  return NULL;      // 해당 key값을 가진 노드 없음
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  if (!t || t->root == t->nil) return NULL; // 널 가드 및 빈 트리 대비

  node_t *cur = t->root;

  while (cur->left != t->nil) {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (!t || t->root == t->nil) return NULL; // 널 가드 및 빈 트리 대비

  node_t *cur = t->root;

  while (cur->right != t->nil) {
    cur = cur->right;
  }

  return cur;
}

// ------------------------------------------------------------------------------------------------------------------
// 삭제

// 삭제 조정
void eraseFixUp(rbtree *t, node_t *x) {
  // 빠진 black에 대해서 해당 위치에 extra black이 부여되게 된다.
  // 해당 위치에 존재하는 노드 x가 red-black이거나 루트일 경우 색을 black으로 변경해주면 된다.
  // 아닐 경우 몇 가지 과정을 거쳐 조정을 해준다.
  while (x->color == RBTREE_BLACK && x != t->root) {
    if (x == x->parent->left) {   // x가 부모 노드의 왼쪽 자식일 때
      node_t *y = x->parent->right;   // y는 x의 형제 노드

      // case 1, x의 형제 노드가 red일 때
      if (y->color == RBTREE_RED) {
        y->color = RBTREE_BLACK;
        y->parent->color = RBTREE_RED;
        lrotate(t, y->parent);  // 회전하면서 x와 x의 부모, 형제의 관계가 바뀜
        y = x->parent->right;   // case 2, 3, 4를 이어서 조정하기 위해 관계 재정립
      }

      // case 2, x의 형제가 black이고 그 자식 노드들도 black일 때
      if (y->left->color == RBTREE_BLACK && y->right->color == RBTREE_BLACK) {
        y->color = RBTREE_RED;
        x = x->parent;    // x의 부모를 기준으로 다시 case 1, 2, 3, 4 확인
      } 
      else {
        // case 3, x의 형제가 black이고 왼쪽 자식만 red일 경우
        if (y->right->color == RBTREE_BLACK) { 
          y->color = RBTREE_RED;
          y->left->color = RBTREE_BLACK;
          rrotate(t, y);    // 회전으로 인해 x와 y의 관계가 바뀜
          y = x->parent->right;
        }
        // case 4, x의 형제가 black이고 오른쪽 자식이 red일 경우
        y->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        y->right->color = RBTREE_BLACK;
        lrotate(t, x->parent);
        x = t->root;  // root 값 black으로 변경
        }

    } else {                      // x가 부모 노드의 오른쪽 자식일 때
      node_t *y = x->parent->left;   // y는 x의 형제 노드

      // case 1, x의 형제 노드가 red일 때
      if (y->color == RBTREE_RED) {
        y->color = RBTREE_BLACK;
        y->parent->color = RBTREE_RED;
        rrotate(t, y->parent);  // 회전하면서 x와 x의 부모, 형제의 관계가 바뀜
        y = x->parent->left;   // case 2, 3, 4를 이어서 조정하기 위해 관계 재정립
      }

      // case 2, x의 형제가 black이고 그 자식 노드들도 black일 때
      if (y->left->color == RBTREE_BLACK && y->right->color == RBTREE_BLACK) {
        y->color = RBTREE_RED;
        x = x->parent;    // x의 부모를 기준으로 다시 case 1, 2, 3, 4 확인
      } 
      else {
        // case 3, x의 형제가 black이고 오른쪽 자식만 red일 경우
        if (y->left->color == RBTREE_BLACK) { 
          y->color = RBTREE_RED;
          y->right->color = RBTREE_BLACK;
          lrotate(t, y);    // 회전으로 인해 x와 y의 관계가 바뀜
          y = x->parent->left;
        }
        // case 4, x의 형제가 black이고 왼쪽 자식이 red일 경우
        y->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        y->left->color = RBTREE_BLACK;
        rrotate(t, x->parent);
        x = t->root;  // root 값 black으로 변경
        }
    }
  }

  x->color = RBTREE_BLACK;
}

// u노드를 v노드로 대체하는 함수
void transplant(rbtree *t, node_t *u, node_t *v) {
  if (!t || u == t->nil) return;
  
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }


  v->parent = u->parent;

  // 만일 제거하려는 노드의 자식이 없으면 nil 노드가 그 자리를 대체한다.
  // nil 노드의 parent는 삭제 조정에서는 필요하므로 여기선 갱신해준다.
}

// successor을 찾는 함수
node_t *findSuccessor(rbtree *t, node_t *p) {
  node_t *s = p->right;   // s : 노드 p의 successor
  while(s->left != t->nil) {
    s = s->left;
  }
  return s;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  if (!t || p == t->nil) return -1;

  //1. 삭제하는 노드의 색
  color_t erasedColor = p->color;

  node_t *z;  // extra-double을 추가해 줄 노드 위치(black-height에 영향을 주는 노드 위치)

  // 2. 노드 삭제
  // 2.0 삭제하려는 노드의 자식이 없을 경우 p의 자식 위치에 존재하는 nil 노드가 p 자리를 대체
  if (p->left == t->nil) {            // 2.1 왼쪽 자식이 없으면 p의 자리를 오른쪽 자식이 대체
    z = p->right;
    transplant(t, p, p->right);
  } else if (p->right == t->nil) {    // 2.2 오른쪽 자식이 없으면 p의 자리를 왼쪽 자식이 대체
    z = p->left;
    transplant(t, p, p->left);
  } else {                            // 2.3 자식이 둘이라면 successor가 p자리를 대체
    node_t *s = findSuccessor(t, p);  // s : 삭제하려는 노드(p)의 successor
    
    z = s->right;
    if (s != p->right) {            // successor가 노드 p의 바로 오른쪽에 위치해 있지 않으면
      transplant(t, s, s->right);   // successor의 오른쪽 자식(서브 트리)을 successor 위치로 이식
      s->right = p->right;          // successor와 기존 노드(p)의 오른쪽 자식을 연결
      s->right->parent = s;
    } else {                        // successor가 삭제하려는 노드 바로 오른쪽일 때
      z->parent = s;                // successor의 오른쪽 자식이 nil 노드일 경우 대비
    }   // 삭제 조정에선 nil 노드의 부모 요소가 필요, 여기선 transplant 함수를 사용 안 하기 때문에 직접 연결

    transplant(t, p, s);    // successor를 삭제하려는 노드(p)의 위치에 이식
    s->left = p->left;      // successor와 기존 노드(p)의 왼쪽 자식을 연결
    s->left->parent = s;

    // 삭제된 노드의 색깔을 successor의 기존 색으로 갱신
    erasedColor = s->color;

    // successor의 색을 기존 노드(p)의 색으로 변경
    s->color = p->color;
  }
  // 2.4 할당된 메모리 해제, 노드 삭제
  free(p);

  // 3. 만약 삭제된 색이 black이라 RBTree 위반이 발생할 경우 조정
  if (erasedColor == RBTREE_BLACK) {
    eraseFixUp(t, z);
  }

  return 0;
}

// ------------------------------------------------------------------------------------------------------------------

// RBTree의 노드 값들을 크기가 n인 배열에 오름차순으로 정렬

void inorder(const rbtree *t, key_t *arr, const size_t n, const node_t *root, size_t *index) {
  if (root == t->nil) return; // 빈 서브트리면 종료, 기저조건
  
  inorder(t, arr, n, root->left, index);
  // 배열을 index 0부터 n-1까지 채운다.
  if ((*index) < n) {         // 아직 배열을 전부 채우지 않았으면
    arr[*index] = root->key;
    (*index)++;
  }
  if ((*index) == n) return;  // 배열의 인덱스 범위 n-1까지 다 채워넣었으면 순회 종료 

  inorder(t, arr, n, root->right, index);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (!t || !arr) return -1;  // 에러 발생 시 -1 반환

  size_t index = 0;
  inorder(t, arr, n, t->root, &index);

  return 0;
}