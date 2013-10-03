//
//  main.cpp
//  ololo1
//
//  Created by Oleg Baskakov on 7/24/13.
//  Copyright (c) 2013 Oleg Baskakov. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <map>

#include "common.h"
#include "inverted_index.h"
#include "wrap.h"


using namespace std;


InvertedIndex inv_index;

class Analyser {
private:
    set<string> stop_words;
public:

    Analyser()
    {
        // stop_words.clean();
        cout << "stop words loading... " << endl;
        ifstream in("stop_stemmed_uniq.txt");
        string line;
        while (in.good())
        {
            getline(in, line);
            stop_words.insert(line);
        }
        in.close();
        // cout << "non_stop: " << non_stop("-") << endl;
        cout << "stop words ok." << endl;
    }

    bool
    non_stop(const string &ss) const
    {
        return !stop_words.count(ss);
    }

} analyser;



void
show_answer2(InvertedIndex &index, vector<string> &raw_data);

void
process_words_del(ifstream &my_file, InvertedIndex &index)
{
    istream_iterator<string> word_iter(my_file);
    istream_iterator<string> word_iter_end;
    for ( ; word_iter != word_iter_end; ++ word_iter ) {
        index.add_word(*word_iter, 1);
    }

}

const char TOKENIZE[] = " ,.\n\t\"\\[]():;`~?/|<>!@#$^&*";


void
process_text(char *_id, char *text, InvertedIndex &index)
{
    inv_index.add_document(_id);
    // ofstream dumper("dump.txt");
    // dumper << ">>> " << string(_id) << endl;
    char *pch;
    // tokenization
    pch = NULL;
    char *next;
    next = strtok_r(text, TOKENIZE, &pch);
    while (next != NULL) {
        tolower_stem_porter(next);
        if (analyser.non_stop(next)) {
            index.add_word(next, 0);
            // dumper << string(next) << " ";
        }
        next = strtok_r(NULL, TOKENIZE, &pch);
    }
    // dumper << endl;
    // cout << endl;
}


void
process_lines(ifstream &my_file,
              InvertedIndex &index,
              vector<string> &raw_data)
{
    string line;
    int line_number = 0;
    while (my_file.good())
    {
        getline(my_file, line);
//        cout << "line: " << line << endl;
        raw_data.push_back(line);
        size_t s_pos = 0;
        size_t f_pos = line.find(" ");
//      TODO make iterator
        int count = 0;
        while (f_pos != -1 and count < 100) {
            count++;
            if (f_pos - s_pos > 2)
                index.add_word(line.substr(s_pos, f_pos - s_pos), line_number);
            s_pos = f_pos + 1;
            f_pos = line.find(" ", s_pos + 1);
        }
        if (line.length() - s_pos > 2)
            index.add_word(line.substr(s_pos), line_number);
        if (count == 100)
            exit(1);
        line_number++;
        index.add_document();
    }
}


void
test_me()
{
    vector<int> arr1;
    arr1.push_back(5);
    arr1.push_back(10);
    arr1.push_back(15);
    arr1.push_back(20);
    arr1.push_back(25);
    vector<int> arr2;
    arr2.push_back(1);
    arr2.push_back(5);
    arr2.push_back(15);
    arr2.push_back(20);
    arr2.push_back(25);
    vector<int> arr3;
    arr3.push_back(5);
    arr3.push_back(15);
    arr3.push_back(25);
    arr3.push_back(30);

    vector<int> res(arr1.size());
    auto it1 = set_intersection(arr1.begin(), arr1.end(), arr2.begin(), arr2.end(), res.begin());
    res.resize(it1 - res.begin());
    auto it2 = set_intersection(res.begin(), res.end(), arr3.begin(), arr3.end(), res.begin());
    res.resize(it2 - res.begin());

    vector<string> resx;
    resx.push_back("dsad");
    resx.push_back("dsads1");
    for (auto &x: resx) {
        cout << x << " ";
    }
    cout << endl;

//    cout << "count: " << m1.count("x") << endl;
//    cout << "count: " << m1.count("z") << endl;

}


const vector<double>
ranking(const vector<int> &vec, const InvertedIndex &index, const forward_list<string> &terms)
{
    vector<double> ranks_vec;
    for (auto &doc_id : vec) {
        double score = 0.0;
        for (auto &term : terms) {
            score += index.tf_idf(term, doc_id);
        }
        ranks_vec.push_back(score);
    }
    return ranks_vec;
}


int
main(int argc, const char *argv[])
{
//    test_me();
//    return 0;
    cout << "stage -2\n";
    inv_index.load_all("tmp.idx");
    cout << "stage -1\n";
    return 0;

    vector<string> raw_data;
    auto file_name = "/Users/oleg/ololo1/example.txt";
    ifstream my_file(file_name);

    cout << "stage0\n";

    if (my_file.is_open())
    {
        process_lines(my_file, inv_index, raw_data);
        my_file.close();
    } else {
        cout << "Unable to open file: " << file_name << endl;
    }
    for (auto x : raw_data) {
        cout << x;
    }
    cout << "stage1" << endl;
    inv_index.show_index("fds");
    cout << "-----" << endl;
    forward_list<string> terms;
    terms.push_front("dsa");
    terms.push_front("fds");
    const auto &res1 = inv_index.posting_intersect(terms);
    cout << "stage2" << endl;
    for (auto &x: res1) {
        cout << raw_data[x] << endl;
    }
    cout << "-----" << endl;
    show_answer2(inv_index, raw_data);

    // insert code here...
    cout << "Hello, World!\n";
    return 0;
}

void
show_answer2(InvertedIndex &index, vector<string> &raw_data)
{
    forward_list<string> terms;
    terms.push_front("dsa");
    terms.push_front("fds");
    const auto &res2 = index.posting_union(terms);
    const auto &ranks2 = ranking(res2, index, terms);
    cout << "stage3" << endl;
    auto doc_id = res2.begin();
    auto rank = ranks2.begin();
    for ( ; doc_id != res2.end() and rank != ranks2.end(); ++doc_id, ++rank) {
        cout << *rank << ":\t" << raw_data[*doc_id] << endl;
    }
    cout << "-----" << endl;
}


int
apiSaveIndex(char *path)
{
    string idx_path(path);
    inv_index.save_all(path);
    return 7;
}

int
apiLoadIndex(char *path)
{
    InvertedIndex tmp_dx;
    inv_index = tmp_dx;
    string idx_path(path);
    inv_index.load_all(path);
    return 7;
}

int
apiAddDocument(char *_id, char *data)
{
    process_text(_id, data, inv_index);
    return 7;
}

static std::string search_query_res;


const char *
apiSearchQuery(char *data, int num_max, int f_union)
{
    // cout << "START " << string(data) << ";" << endl;
    forward_list<string> query;
    char *pch;
    pch = strtok(data, " ");
    while (pch != NULL) {
        tolower_stem_porter(pch);
        if (analyser.non_stop(pch)) {
            // TODO reverse order
            // cout << "push_front " << string(pch) << ";" << endl;
            query.push_front(pch);
        }
        pch = strtok(NULL, " ");
    }
    cout << "QUERY ->" << " ";
    print_forward_list(query);
    // cout << "posting_intersect ->" << endl;
    // print_vec(res1);
    // for (auto &x: res1) {
    //     cout << x << endl;
    // }
    // cout << "-----" << endl;
    vector<int> res2;
    if (f_union) {
        res2 = inv_index.posting_union(query);
    } else {
        res2 = inv_index.posting_intersect(query);
    }
    vector<double> ranks2 = ranking(res2, inv_index, query);

    // std::vector<int> keys;
    // std::vector<double> vals;
    // if (num > res2.size()) {
    //     num = res2.size();
    // }
    // keys.assign(res2.begin(), res2.begin() + num_max);
    // vals.assign(ranks2.begin(), ranks2.begin() + num_max);
    // double pivot = *std::min_element(ranks2.begin(), ranks2.begin() + num_max);
    
    vector<pair<double,int> > ololo;
    for (int i = 0; i < res2.size(); ++i) {
        ololo.push_back(make_pair(-ranks2[i], res2[i])); // for reverse sort
    }
    std::partial_sort(ololo.begin(), ololo.begin() + num_max, ololo.end());
    ololo.resize(num_max);

    // vector<string> document_ids;
    // for (auto num_id : res2) {
    //     document_ids.push_back(inv_index.get_document(num_id));
    // }

    // make JSON
    search_query_res = "[\n";
    for (const auto &doc_p : ololo) {
        search_query_res += "(\"";
        search_query_res += inv_index.get_document(doc_p.second);
        search_query_res += "\", ";
        char buf[20];
        snprintf(buf, 20, "%lf", -doc_p.first);
        search_query_res += buf;
        search_query_res += "),\n";
    }
    // delete last comma
    if (ololo.size() != 0)
        search_query_res[search_query_res.length() - 2] = ']';
    else
        search_query_res[search_query_res.length() - 1] = ']';
    return search_query_res.data();
}


int
apiShowIndex(char *token)
{
    tolower_stem_porter(token);
    if (analyser.non_stop(token))
        inv_index.show_index(token);
    return 7;
}

















