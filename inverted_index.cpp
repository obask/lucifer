//
//  inv_index.cpp
//  ololo1
//
//  Created by Oleg Baskakov on 7/24/13.
//  Copyright (c) 2013 Oleg Baskakov. All rights reserved.
//

#include "inverted_index.h"
#include "common.h"

// a predicate implemented as a class:
class is_odd_class
{
    map<string, vector<int> > *postings;
public:
    is_odd_class(map<string, vector<int> > *_postings)
    :postings(_postings)
    {}

    bool
    operator() (const string &s) {return postings->count(s) == 0; }

};

vector<int>
InvertedIndex::posting_union(forward_list<string> doc_list)
{
    std::cout << "InvertedIndex::posting_union" << endl;
    // filter keys not from index
    doc_list.remove_if(is_odd_class(&postings));
    // doc_list.remove_if([&](string &s) { return postings.count(s) == 0; });
    // assert list not empty
    if (doc_list.empty())
        return vector<int>();
    // reduce union
    vector<int> last = postings[doc_list.front()];
    vector<int> res;
    doc_list.pop_front();
    for (const auto &elem : doc_list) {
        const auto &arr = postings[elem];
        std::set_union(last.begin(), last.end(),
                       arr.begin(), arr.end(),
                       std::back_inserter(res));
        last.swap(res);
        res.clear();
    }
    return last;
}

vector<int>
InvertedIndex::posting_intersect(forward_list<string> doc_list)
{
    // assert list not empty
    if (doc_list.empty())
        return vector<int>();
    // filter keys not from index
    for (auto &elem : doc_list) {
        if (postings.count(elem) == 0)
            return vector<int>();
    }
    // reduce intersection
    vector<int> res = postings[doc_list.front()];
    doc_list.pop_front();
    for (auto &elem : doc_list) {
        auto &arr = postings[elem];
        auto it = std::set_intersection(res.begin(), res.end(),
                arr.begin(), arr.end(), res.begin());
        res.resize(it - res.begin());
    }
    return res;
}






int
InvertedIndex::save_all(string idx_path)
{
    ofstream out(idx_path, std::ofstream::binary);
    size_t documents_number = doc_ids.size();
    save_size_t(out, documents_number);
    for (auto &doc : doc_ids) {
        save_string(out, doc);
    }
    size_t dict_size = words_dict.size();
    save_size_t(out, dict_size);
    for (auto &x : words_dict) {
        auto &token = x.first;
        save_string(out, token);
        // word count
        size_t token_count = x.second;
        assert(token_count > 0);
        save_size_t(out, token_count);
        // posting list
        auto &posting_vec = postings.at(token);
        size_t postings_count = posting_vec.size();
        assert(postings_count > 0);
        save_size_t(out, postings_count);
        out.write((const char *)posting_vec.data(), postings_count * sizeof(posting_vec[0]));
        // tf list
        auto &tf_values_vec = tf_values.at(token);
        size_t tf_values_count = tf_values_vec.size();
        assert(tf_values_count > 0);
        save_size_t(out, tf_values_count);
        out.write((const char *)tf_values_vec.data(), tf_values_count * sizeof(tf_values_vec[0]));
    }
    // checksum
    size_t checksum = doc_ids.size() + words_dict.size();
    save_size_t(out, checksum);
    return words_dict.size();
}



int
InvertedIndex::load_all(string idx_path)
{
    cout << "loading..." << endl;
    ifstream in(idx_path, std::ifstream::binary);
    size_t documents_number;
    in.read((char *)&documents_number, sizeof(size_t));
    doc_ids.clear();
    for (size_t i=0; i < documents_number; ++i) {
        string tmp;
        load_string(in, tmp);
        doc_ids.push_back(tmp);
    }
    size_t tokens_number;
    in.read((char *)&tokens_number, sizeof(size_t));
    for (int i = 0; i < tokens_number; ++i) {
        string token;
        load_string(in, token);
        // word count
        size_t token_count;
        in.read((char *)&token_count, sizeof(size_t));
        words_dict[token] = token_count;
        // posting list
        auto &posting_vec = postings[token] = vector<int>();
        size_t postings_count;
        in.read((char *)&postings_count, sizeof(size_t));
        posting_vec.resize(postings_count);
        in.read((char *)posting_vec.data(), postings_count * sizeof(int));
        // tf list
        auto &tf_values_vec = tf_values[token] = vector<int>();
        size_t tf_values_count;
        in.read((char *)&tf_values_count, sizeof(size_t));
        tf_values_vec.resize(tf_values_count);
        in.read((char *)tf_values_vec.data(), tf_values_count * sizeof(int));
    }
    size_t checksum;
    in.read((char *)&checksum, sizeof(size_t));
    checksum += 10;
    assert(checksum == tokens_number + documents_number + 10);
    cout << "loading... ok" << endl;
    return tokens_number;
}






