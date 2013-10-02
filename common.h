#ifndef __ololo1__common__
#define __ololo1__common__

#include <vector>
#include <forward_list>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void
print_vec(const vector<int> &vec);

inline void
print_forward_list(const forward_list<string> &vec)
{
    for (auto &x : vec)
        cout << x << " ";
    cout << ";" << endl;
}


#define save_size_t(out, num) out.write((const char *)&num, sizeof(size_t));


// #define save_string(out, ss) \
// { \
//     size_t ss_len = ss.size();\
//     save_size_t(out, ss_len);\
//     out.write(ss.data(), ss_len);\
// }


inline void
save_string(ofstream &out, const string &ss)
{
    size_t ss_len = ss.size();
    out.write((const char *)&ss_len, sizeof(size_t));
    out.write(ss.data(), ss_len);
}


#define load_string(in, strx) \
{ \
    size_t __token_len; \
    in.read((char *)&__token_len, sizeof(size_t)); \
    char ctoken[__token_len+1]; \
    in.read(ctoken, __token_len); \
    strx = string(ctoken, __token_len); \
}




#endif /* defined(__ololo1__common__) */
