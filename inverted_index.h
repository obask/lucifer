//
//  inv_index.h
//  ololo1
//
//  Created by Oleg Baskakov on 7/24/13.
//  Copyright (c) 2013 Oleg Baskakov. All rights reserved.
//

#ifndef __ololo1__inv_index__
#define __ololo1__inv_index__

#include <algorithm>
#include <cmath>
#include <cassert>
#include <map>
#include <set>
#include <vector>
#include <forward_list>
#include <string>
#include <iostream>
#include <fstream>

#include "common.h"

using namespace std;

class InvertedIndex {

public:

    double
    tf_idf(string term, int doc_id) const
    {
        // cout << "count_tf: " << count_tf(term, doc_id) << endl;
        // cout << "count_idf: " << count_idf(term) << endl;
        // cout << "words num: " << words_dict.at(term) << endl;
        // cout << "postings.at(term).size(): " << postings.at(term).size() << endl;
        return count_tf(term, doc_id) * count_idf(term);
    }

    void
    add_document(string title="")
    {
        doc_ids.push_back(title);
    }

    string
    get_document(int num) const
    {
        return doc_ids.at(num);
    }

    void
    add_word(string term, int doc_id)
    {
        doc_id = doc_ids.size() - 1;
        if (postings.count(term) == 0) {
//            cout << "add_word: " << term << " " << pos << endl;
            postings[term] = vector<int>(1, doc_id);
            tf_values[term] = vector<int>(1, 1);
            words_dict[term] = 1;
        } else {
            auto &x = postings[term];
            auto &tf_vec = tf_values[term];
            assert(x.size() == tf_vec.size());
            if (x[x.size() - 1] != doc_id) {
                x.push_back(doc_id);
                tf_vec.push_back(0);
            }
            tf_vec[tf_vec.size() - 1] += 1;
            words_dict[term]++;
        }
    }


    vector<int>
    posting_intersect(forward_list<string> doc_list);

    vector<int>
    posting_union(forward_list<string> doc_list);

    int
    save_all(string idx_path);

    int
    load_all(string idx_path);




    void
    show_index(string tst)
    {
        cout << "debug: " << tst << endl;
        cout << "df = ";
        cout << words_dict[tst] << endl;
        cout << "postings: ";
        print_vec(postings[tst]);
        // cout << endl;
        cout << "tf_values: ";
        print_vec(tf_values[tst]);
        // cout << endl;

        // cout << "----------" << endl;
        // cout << "index:" << endl;
        // for (auto &x : postings) {
        //     cout << x.first << ": ";
        //     print_vec(x.second);
        // }
        // cout << "----------" << endl;
        // cout << "tf_values:" << endl;
        // for (auto &x : tf_values) {
        //     cout << x.first << ": ";
        //     print_vec(x.second);
        // }
        // cout << "----------" << endl;
    }


private:

    double
    count_tf(string term, int doc_id) const
    {
        const auto &v = postings.at(term);
        if (std::binary_search (v.begin(), v.end(), doc_id)) {
            auto i = std::lower_bound(v.begin(), v.end(), doc_id) - v.begin();
            auto &tf = tf_values.at(term)[i];
            return 1.0 + log(1.0 * tf);
        } else {
            return 0.0;
        }
    }

    double
    count_idf(string term) const
    {
        return log(1.0 * words_dict.size() / (postings.at(term).size() + 1));
    }

    map<string, vector<int> > postings;
    map<string, vector<int> > tf_values;
    map<string, int > words_dict;
    vector<string> doc_ids;

};



#endif /* defined(__ololo1__inv_index__) */
