# 고급 시스템 프로그래밍 Advanced System Programming

국민대학교 전자공학부 20142957 이은총
 
File

- gen.c   : n 개의  m MB의 크기의 text 파일 만들기 (file merge용)
- merge.c : merge.c 두개의 파일을 merge 하고 시간을 재는 예제 소스 
- merge_array.c : 배열을 이용한 함수, 가장 성능이 우수하다.
- merge_arraystack.c : 배열 스택을 이용한 함수.
- merge_linkedstack.c : 연결 리스트 스택을 이용한 함수.

USAGE

- ./gen 2 100
- ls -l /tmp/file_*
- ./merge_* /tmp/file_0001 /tmp/file_0002 f_out
- ls -l f_out

