#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;

typedef long long pos_t;
typedef int depth_t;
typedef int sufix_t;
typedef long long bsize_t;
typedef string filename_t;


//CHANGE TO TEMPLATE
typedef int T;


struct Record{
};

struct Bucket {
    bsize_t bsize;
    pos_t next_bucket;
    vector<Record> records;

    void load_from_file(fstream &file, pos_t bucket_pos) {
        file.seekg(bucket_pos, ios::beg);
        file.read( (char*) &(this->bsize), sizeof(bsize_t) );
        file.read( (char*) &(this->next_bucket), sizeof(pos_t) );
        Record record;
        for (int i=0; i<bsize; i++) {
            file.read( (char*) &(record), sizeof(Record) );
            records.push_back(record);
        }
    }
    void save_to_file(fstream &file, pos_t bucket_pos) {
        file.seekp(bucket_pos, ios::beg);
        file.write( (char*) &(this->bsize), sizeof(bsize_t) );
        file.write( (char*) &(this->next_bucket), sizeof(pos_t) );
        for (auto &record : records) {
            file.write( (char*) &(record), sizeof(Record) );
        }
    }
};


struct IndexRecord {
    depth_t depth;
    sufix_t sufix;
    pos_t bucket_pos;
    void load_from_ifile(ifstream &file) {
        file.read( (char*) &(this->depth), sizeof(depth_t) );
        file.read( (char*) &(this->sufix), sizeof(sufix_t) );
        file.read( (char*) &(this->bucket_pos), sizeof(pos_t) );
    }
    void save_to_ofile(ofstream &file) {
        file.write( (char*) &(this->depth), sizeof(depth_t) );
        file.write( (char*) &(this->sufix), sizeof(sufix_t) );
        file.write( (char*) &(this->bucket_pos), sizeof(pos_t) );
    }
};

class ExtensibleHash {
    private:
        // Files
        filename_t filename_index;
        filename_t filename_data;
        // Index
        vector<IndexRecord> index_table;
        depth_t max_depth;
        // Index Add Entry
        void add_entry_index(IndexRecord irecord) {
            if (max_depth < irecord.depth){
                max_depth=irecord.depth;
            }
            index_table.push_back(irecord);
        }
        // Index init
        void init_index() {
            // One bucket with prefix=0. Another bucket with prefix=1.
            IndexRecord init0{1,0,-1};
            IndexRecord init1{1,1,-1};
            add_entry_index(init0);
            add_entry_index(init1);
        }
        // Index load
        void load_index() {
            ifstream input_index(filename_index, ios::binary);
            max_depth = 0; // Start depth at 0
            bool file_exists = input_index.is_open();
            if (file_exists){
                IndexRecord irecord;
                irecord.load_from_ifile(input_index); // Load record
                while(input_index) {
                    add_entry_index(irecord);
                    irecord.load_from_ifile(input_index); // Load next record
                }
            }
            input_index.close();
            // If file does not exist, init basic index
            if (!file_exists) {
                init_index();
            }
        }
        // Index save
        void save_index() {
            ofstream out_index(filename_index, ios::binary | ios::trunc);
            if (out_index.is_open()) {
                for (auto &irecord : index_table) {
                    irecord.save_to_ofile(out_index);
                }
            }
            out_index.close();
        }
        // Compare Hash
        bool validate_hash(sufix_t hash, sufix_t sufix, depth_t depth) {
            for (int i = 0; i<depth; i++) {
                sufix_t hash_bit = hash%2; hash = hash/2; // Extract last bit
                sufix_t sufix_bit = sufix%2; sufix = sufix/2; // Extract last bit
                if (hash_bit != sufix_bit) {
                    return false;
                }
            }
            return true;
        }
    public:
        // Write new entry
        bool add(Record record) {
            // TODO: add
        }
        // Remove entry
        bool remove(T key) {
            // TODO: remove
        }
        // Search entry
        vector<Record> search(T key) {
            // TODO: search

        }
        // SearchRange entry
        vector<Record> rangeSearch(T begin_key, T end_key) {
            // TODO: searchRange
        }
};

