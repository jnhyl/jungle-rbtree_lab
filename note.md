## 크래프톤 정글 RBTree 과정의 개인 풀이를 올려놓은 public 저장소입니다.
---
private 저장소에 올려놓은 버전이 있으나 rb-tree-visualizer\(이하 visualizer라고 칭함\) 프로그램으로 인해 폴더 구조가 처음이랑 많이 달라져서 아예 새로 만들어 public 저장소에 올립니다.
아직 visualizer 프로그램에 익숙하지 않습니다. 충분히 숙지했다고 생각이 되면 visualizer 프로그램을 사용한 버전 및 테스트 코드도 public 저장소에 올려보겠습니다.

해당 코드는 sentinel 버전으로 구현했습니다. 혹시 test 폴더의 makefile 내용에서 -DSENTINEL이 주석 처리가 되어있다면 해제해주세요.

후반부 함수들, 특히 erase 함수는 clrs 교재의 의사 코드를 기준으로 작성하였으나 전반부의 삽입 함수와 헬퍼들\(lrotate, rrotate\), 그리고 탐색\(find, min, max\) 함수들은 교재와 다를 수 있습니다.
예를 들어 insert 함수는 원래 중복을 허용하지 않는 버전으로 구현하였으나 테스트 코드 및 교재 기준으로 다시 수정하였습니다. \(중복 허용\)
또한 함수들의 NULL Guard 및 조건 부적합 시 반환하는 값을 t.nil에서 NULL로 수정했습니다.\(테스트 코드 기준\)

rb-tree-visualizer는 구현한 함수를 시각화해주는 프로그램입니다. 크래프톤 정글 9기 유호준, 김기래 님
