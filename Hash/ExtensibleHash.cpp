#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <stdio.h>
#include <chrono>
#include <sys/time.h>
#include <ctime>
// Comentar o descomentar para Activar/Desactivar mensajes de Debug

//#define DEBUG_ACTIONS // Show start and end of actions
//#define DEBUG_ADD_TREE // Show add decision tree
//#define DEBUG_SHOW_INDEX // Show index at start of actions
//#define DEBUG_SEARCH_TREE // Show data of search tree
//#define DEBUG_INSERT_OVERFLOW // Show data of overflow pages creation
//#define DEBUG_INSERT_WRITEBACK // Show data of bucket writeback
//#define DEBUG_RANGESEARCH_STEP // Show rangesearch vector data in each step (bucket)
//#define DEBUG_SPLIT_RECORDS // Show records after split

using namespace std;

typedef long long pos_t;
typedef int depth_t;
typedef int sufix_t;
typedef long long bsize_t;
typedef string filename_t;

//TODO: Record structure
typedef string T;
struct Record{

    int codigo;
    int FIPS_code;
    char estado[30];
    char date[11];
    int totaldeath;
    int confirmedCase;
    long longitd;
    long latitud;

/*
    char nombre[20];
    char apellido[20];
    char codigo[10];
    char carrera[10];
    int edad;
    int ciclo;
    int creditos;
*/
    void set_data_from_string(string _params) {
        stringstream ss(_params);
        string _codigo;
        string _FIPS_code;
        string _estado;
        string _date;
        string _totaldeath;
        string _confirmedCase;
        string _longitd;
        string _latitud;
        getline(ss, _codigo, ',');
        getline(ss, _FIPS_code, ',');
        getline(ss, _estado, ',');
        getline(ss, _date, ',');
        getline(ss, _totaldeath, ',');
        getline(ss, _confirmedCase, ',');
        getline(ss, _longitd, ',');
        getline(ss, _latitud, ',');

        set_data(stoi(_codigo),stoi(_FIPS_code),_estado,_date, stoi(_totaldeath), stoi(_confirmedCase), stol(_longitd), stol(_latitud));
    }

    void set_data(int _codigo, int _FIPS_code, string _estado, string _date, int _totaldeath, int _confirmedCase, long _longitd, long _latitud){
        codigo = _codigo;
        FIPS_code = _FIPS_code;
        strcpy(estado, _estado.c_str());
        strcpy(date, _date.c_str());
        totaldeath = _totaldeath;
        confirmedCase = _confirmedCase;
        longitd = _longitd;
        latitud = _latitud;

    }

    void print_data(){
        cout << codigo<< " | "
        << FIPS_code << " | "
        << string(estado) << " | "
        << string(date) << " | "
        << totaldeath << " | "
        << confirmedCase << " | "
        << longitd << " | "
        << latitud;
    }

    void load_from_file(fstream &file) {
        file.read( (char*) &codigo, sizeof(codigo) );
        file.read( (char*) &FIPS_code, sizeof(FIPS_code) );
        file.read( (char*) estado, sizeof(estado) );
        file.read( (char*) date, sizeof(date) );
        file.read( (char*) &totaldeath, sizeof(totaldeath) );
        file.read( (char*) &confirmedCase, sizeof(confirmedCase) );
        file.read( (char*) &longitd, sizeof(longitd) );
        file.read( (char*) &latitud, sizeof(latitud) );

    }

    void save_to_file(fstream &file) {
        file.write( (char*) &codigo, sizeof(codigo) );
        file.write( (char*) &FIPS_code, sizeof(FIPS_code) );
        file.write( (char*) estado, sizeof(estado) );
        file.write( (char*) date, sizeof(date) );
        file.write( (char*) &totaldeath, sizeof(totaldeath) );
        file.write( (char*) &confirmedCase, sizeof(confirmedCase) );
        file.write( (char*) &longitd, sizeof(longitd) );
        file.write( (char*) &latitud, sizeof(latitud) );

    }

    // DUPLICATE FOR OFSTREAM
    void save_to_file(ofstream &file) {
        file.write( (char*) &codigo, sizeof(codigo) );
        file.write( (char*) &FIPS_code, sizeof(FIPS_code) );
        file.write( (char*) estado, sizeof(estado) );
        file.write( (char*) date, sizeof(date) );
        file.write( (char*) &totaldeath, sizeof(totaldeath) );
        file.write( (char*) &confirmedCase, sizeof(confirmedCase) );
        file.write( (char*) &longitd, sizeof(longitd) );
        file.write( (char*) &latitud, sizeof(latitud) );

    }

    T get_key() {

        return to_string(codigo);
    }

};

struct Bucket {
    bsize_t bsize;
    pos_t next_bucket;
    vector<Record> records;

    void load_from_file(fstream &file, pos_t bucket_pos, bsize_t bucket_size) {
        file.seekg(bucket_pos, ios::beg);
        file.read( (char*) &(this->bsize), sizeof(bsize_t) );
        file.read( (char*) &(this->next_bucket), sizeof(pos_t) );
        // Load to vector only valid data
        vector<Record> tmp;
        Record record;
        for (int i=0; i<bsize; i++) {
            record.load_from_file(file);
            tmp.push_back(record);
        }
        records = tmp;
    }
    void save_to_file(fstream &file, pos_t bucket_pos, bsize_t bucket_size) {
        file.seekp(bucket_pos, ios::beg);
        file.write( (char*) &(this->bsize), sizeof(bsize_t) );
        file.write( (char*) &(this->next_bucket), sizeof(pos_t) );
        // Write all to reserve space
        Record record;
        for (int i=0; i<bucket_size; i++) {
            if (i<records.size()) {
                record = records[i];
            }
            record.save_to_file(file);
        }
    }

    // DUPLICATE FOR OFSTREAM
    void save_to_file(ofstream &file, bsize_t bucket_size) {
        file.write( (char*) &(this->bsize), sizeof(bsize_t) );
        file.write( (char*) &(this->next_bucket), sizeof(pos_t) );
        // Write all to reserve space
        Record record;
        for (int i=0; i<bucket_size; i++) {
            if (i<records.size()) {
                record = records[i];
            }
            record.save_to_file(file);
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
    void print_data() {
        cout << "Depth: " << depth << " | "
             << "Sufix: " << sufix << " | "
             << "BucketP: " << bucket_pos; 
    }
};

class ExtensibleHash {
    private:
        // Bucket size
        bsize_t bucket_max_size;
        // Files
        filename_t filename_index;
        filename_t filename_data;
        // Index
        vector<IndexRecord> index_table;
        depth_t max_depth;
        // Index Add Entry
        void add_entry_index(IndexRecord irecord) {
            //if (global_depth < irecord.depth){
            //    global_depth=irecord.depth;
            //}
            index_table.push_back(irecord);
        }
        // Index init
        void init_index() {
            // Empty Buckets
            Bucket empty_bucket{0,-1};
            // Reserve buckets in data file
            pos_t pos0, pos1;
            ofstream data_file(filename_data, ios::binary | ios::trunc);
            if (data_file.is_open()) {
                // Write two copies of empty buckets
                pos0=0;
                empty_bucket.save_to_file(data_file, bucket_max_size);
                pos1=data_file.tellp();
                empty_bucket.save_to_file(data_file, bucket_max_size);
                // Close file
                data_file.close();
            }
            else {
                data_file.close();
                cout << "[ERROR]: Cant Init Index File" << endl;
                exit(2);
            }
            // One bucket with prefix=0. Another bucket with prefix=1.
            IndexRecord init0{1,0,pos0};
            IndexRecord init1{1,1,pos1};
            add_entry_index(init0);
            add_entry_index(init1);
        }
        // Index load
        void load_index() {
            ifstream input_index(filename_index, ios::binary);
            // global_depth = 0; // Start depth at 0
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
        bool validate_hash(sufix_t key_hash, sufix_t sufix, depth_t depth) {
            for (int i = 0; i<depth; i++) {
                // Standerize modulo operation: (a % n + n) % n [Guarantees always positive]
                sufix_t key_hash_bit = (key_hash%2 + 2) %2; key_hash = key_hash/2; // Extract last bit
                sufix_t sufix_bit = sufix%2; sufix = sufix/2; // Extract last bit
                if (key_hash_bit != sufix_bit) {
                    return false;
                }
            }
            return true;
        }
        // Generate Hash
        sufix_t generate_hash(T key) {
            return std::hash<T>()(key);
        }
        // Get matching index record
        IndexRecord* match_irecord(sufix_t key_hash){
            for (auto &irecord : index_table) {
                if (validate_hash(key_hash, irecord.sufix, irecord.depth)) {
                    // Return pointer to matching record
                    return &irecord;
                }
            }
            // No match 
            cout << "[ERROR]: No matching prefix" <<endl;
            exit(2);
        }
        #ifdef DEBUG_SHOW_INDEX
        void show_index() {
            cout << "[DEBUG]: Current Index" << endl;
            for (auto &ir : index_table) {
                cout << "[DEBUG]: ->";
                ir.print_data();
                cout << endl;
            }
        }
        #endif
    public:
        ExtensibleHash(filename_t filename, bsize_t bucket_size, depth_t depth) {
            filename_data = filename + ".dat";
            filename_index = filename + ".ind";
            bucket_max_size = bucket_size;
            max_depth = depth;
            load_index();
        }
        ~ExtensibleHash() {
            save_index();
        }
        // Write new entry
        bool add(Record record) {
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: Start add" << endl;
            #endif
            #ifdef DEBUG_SHOW_INDEX
            show_index();
            #endif
            // TODO: Add
            // Get hash
            auto key_hash = generate_hash(record.get_key());
            // Search for matching bucket in index table
            auto irecord = match_irecord(key_hash);
            auto bucket_pos = irecord->bucket_pos;
            Bucket bucket;
            fstream data_file(filename_data, ios::binary | ios::out | ios::in);
            if (data_file.is_open()){
                // Load bucket
                bucket.load_from_file(data_file, bucket_pos, bucket_max_size);
                // Check if bucket is full
                if (bucket.bsize == bucket_max_size){
                    // FULL CASE
                    if (irecord->depth < max_depth) {
                        #ifdef DEBUG_ADD_TREE
                        cout << "[DEBUG]: Branch add::SPLIT" << endl;
                        #endif
                        // SPLIT
                        // zro_sufix: 0+sufix (insert in old)
                        // one_sufix: 1+sufix (insert in new)
                        irecord->depth++; // Increase irecord depth by 1
                        // Copy to new vectors
                        vector<Record> zro_sufix;
                        vector<Record> one_sufix;
                        for (auto &rec : bucket.records) {
                            auto rec_hash = generate_hash(rec.get_key());
                            if ( validate_hash(rec_hash, irecord->sufix, irecord->depth ) ) {
                                zro_sufix.push_back(rec);
                            }
                            else {
                                one_sufix.push_back(rec);
                            }
                        }
                        #ifdef DEBUG_SPLIT_RECORDS
                        cout << "[DEBUG]: Split Record records:" << endl;
                        cout << "[DEBUG]: > Zro bucket records" << endl;
                        for (auto &rec : zro_sufix) {
                            cout << "[DEBUG]: -";
                            rec.print_data();
                            cout << endl;
                        }
                        cout << "[DEBUG]: > One bucket records" << endl;
                        for (auto &rec : one_sufix) {
                            cout << "[DEBUG]: -";
                            rec.print_data();
                            cout << endl;
                        }
                        #endif
                        // Split irecord
                        // Overwrite old bucket with 0+sufix
                        bucket.records = zro_sufix;
                        bucket.bsize = zro_sufix.size();
                        // Create and WRITE new irecord-bucket pair for 1+sufix
                        Bucket one_bucket;
                        one_bucket.records = one_sufix;
                        one_bucket.bsize = one_sufix.size();
                        one_bucket.next_bucket = -1;
                        // WRITE 
                        data_file.seekp(0, ios::end); // Go to eof
                        pos_t pos_new = data_file.tellp(); // Save pos
                        one_bucket.save_to_file(data_file, pos_new, bucket_max_size); // Write at eof
                        bucket.save_to_file(data_file, bucket_pos, bucket_max_size); // Overwite old bucket in file
                        // New irecord
                        sufix_t sufix_new = irecord->sufix + (1 << (irecord->depth-1));
                        IndexRecord irecord_new{irecord->depth, sufix_new, pos_new};
                        add_entry_index(irecord_new);
                        // Add record recursievly
                        data_file.close();
                        add(record);
                    }
                    else {
                        #ifdef DEBUG_ADD_TREE
                        cout << "[DEBUG]: Branch add::OVERFLOW_PAGE" << endl;
                        #endif
                        // OVERFLOW PAGE
                        auto prev_bucket_pos = bucket_pos;
                        bucket_pos = bucket.next_bucket;
                        // Try to find and insert in an existing overflow page
                        while (bucket_pos != -1) {
                            bucket.load_from_file(data_file, bucket_pos, bucket_max_size);
                            if (bucket.bsize != bucket_max_size) { break; }
                            prev_bucket_pos = bucket_pos;
                            bucket_pos = bucket.next_bucket;
                        }
                        #ifdef DEBUG_INSERT_OVERFLOW
                        cout << "[DEBUG]: Bucket size: " << bucket.bsize  << ", pos: " << bucket_pos << endl;
                        cout << "[DEBUG]: Bucket real size: " << bucket.records.size() << ", pos: " << bucket_pos << endl;
                        for (auto &rec : bucket.records) {
                            cout << "[DEBUG]: -";
                            rec.print_data();
                            cout << endl;
                        }
                        #endif
                        // If none exist, create new Overflow page.
                        if (bucket_pos == -1) {
                            // In previous bucket, point to end of current file, where new page is created
                            data_file.seekp(0, ios::end);
                            bucket_pos = data_file.tellp();
                            bucket.next_bucket = bucket_pos;
                            bucket.save_to_file(data_file, prev_bucket_pos, bucket_max_size);
                            #ifdef DEBUG_INSERT_OVERFLOW
                            cout << "[DEBUG]: Original Bucket next_bucket: " << bucket.next_bucket << ", pos: " << prev_bucket_pos << endl;
                            #endif
                            // Init empty bucket
                            Bucket empty_bucket{0,-1};
                            bucket = empty_bucket;
                            #ifdef DEBUG_INSERT_OVERFLOW
                            cout << "[DEBUG]: Overflow Bucket next_bucket" << bucket.next_bucket << ", pos: " << bucket_pos  << endl;
                            #endif
                        }
                        // Insert normally
                        bucket.records.push_back(record);
                        bucket.bsize++;
                        #ifdef DEBUG_INSERT_OVERFLOW
                        cout << "[DEBUG]: Bucket size: " << bucket.bsize  << ", pos: " << bucket_pos << endl;
                        cout << "[DEBUG]: Bucket real size: " << bucket.records.size()  << ", pos: " << bucket_pos << endl;
                        for (auto &rec : bucket.records) {
                            cout << "[DEBUG]: -";
                            rec.print_data();
                            cout << endl;
                        }
                        #endif
                    }
                }
                else {
                    // NOTFULL CASE: Simple insert
                    #ifdef DEBUG_ADD_TREE
                    cout << "[DEBUG]: Branch add::SIMPLE_INSERT" << endl;
                    #endif
                    bucket.records.push_back(record);
                    bucket.bsize++;
                }
                // Writeback bucket
                #ifdef DEBUG_INSERT_WRITEBACK
                cout << "[DEBUG]: Writeback Bucket next_bucket" << bucket.next_bucket << ", pos: " << bucket_pos  << endl;
                for (auto &rec : bucket.records) {
                    cout << "[DEBUG]: -";
                    rec.print_data();
                    cout << endl;
                }
                #endif
                bucket.save_to_file(data_file, bucket_pos, bucket_max_size);
                data_file.close();
            }
            else {
                // On fail to open: return false
                data_file.close();
                #ifdef DEBUG_ACTIONS
                cout << "[DEBUG]: End add (Fail open)" << endl;
                #endif
                return false;
            }
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: End add (Success)" << endl;
            #endif
            return true;
        }
        // Remove entry
        bool remove(T key) {
            // TODO: remove
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: Start remove" << endl;
            #endif
            #ifdef DEBUG_SHOW_INDEX
            show_index();
            #endif
            // Get hash
            auto key_hash = generate_hash(key);
            // Search for matching bucket in index table
            auto irecord = match_irecord(key_hash);
            auto bucket_pos = irecord->bucket_pos;
            Bucket bucket;
            // Search bucket and overflow pages (remove ALL aparitions)
            fstream data_file(filename_data, ios::binary | ios::in | ios::out);
            if (data_file.is_open()){
                while (bucket_pos != -1) {
                    bucket.load_from_file(data_file, bucket_pos, bucket_max_size); // Read bucket
                    // Remove repetitions from bucket
                    vector<Record> res;
                    for (auto &record :  bucket.records) {
                        if (record.get_key() != key) {
                            res.push_back(record);
                        }
                    }
                    bucket.records = res;
                    bucket.bsize = res.size();
                    // Writeback bucket
                    bucket.save_to_file(data_file, bucket_pos, bucket_max_size);
                    // Point to next bucket pos
                    bucket_pos = bucket.next_bucket; 
                }
                // Close file
                data_file.close();
            }
            else {
                // On fail to open: return false
                data_file.close();
                #ifdef DEBUG_ACTIONS
                cout << "[DEBUG]: End remove (Fail open)" << endl;
                #endif
                return false;
            }
            // Return
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: End remove (Success)" << endl;
            #endif
            return true;
        }
        
        // Search entry
        vector<Record> search(T key) {
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: Start search" << endl;
            #endif
            #ifdef DEBUG_SHOW_INDEX
            show_index();
            #endif
            // Initialize return vector
            vector<Record> result_records;
            // Get hash
            auto key_hash = generate_hash(key);
            // Search for matching bucket in index table
            auto irecord = match_irecord(key_hash);
            auto bucket_pos = irecord->bucket_pos;
            #ifdef DEBUG_SEARCH_TREE
            cout << "[DEBUG]: Search in irecord: " << irecord->sufix << endl;
            cout << "[DEBUG]: Bucket pos: " << bucket_pos << endl;
            #endif
            Bucket bucket;
            // Search bucket and overflow pages
            fstream data_file(filename_data, ios::binary | ios::in);
            if (data_file.is_open()){
                while (bucket_pos != -1) {
                    bucket.load_from_file(data_file, bucket_pos, bucket_max_size); // Read bucket
                    // Search in bucket
                    for (auto &record :  bucket.records) {
                        if (record.get_key() == key) {
                            result_records.push_back(record);
                        }
                    }
                    bucket_pos = bucket.next_bucket; // Point to next bucket pos
                    #ifdef DEBUG_SEARCH_TREE
                    cout << "[DEBUG]: Next bucket pos: " << bucket_pos << endl;
                    #endif
                }
            }
            #ifdef DEBUG_ACTIONS
            else{
                cout << "[DEBUG]: Search (Fail open)" << endl;
            }
            #endif
            data_file.close();
            // Return
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: End search" << endl;
            #endif
            return result_records;
        }
        // SearchRange entry
        vector<Record> rangeSearch(T begin_key, T end_key) {
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: Start rangeSearch" << endl;
            #endif
            #ifdef DEBUG_SHOW_INDEX
            show_index();
            #endif
            // Initialize return vector
            vector<Record> result_records;
            // Open file
            fstream data_file(filename_data, ios::binary | ios::in);
            if (data_file.is_open()){
                // Iterate over all buckets
                for (auto &irecord : index_table ){
                    auto bucket_pos = irecord.bucket_pos;
                    Bucket bucket;
                    // Search bucket and overflow pages    
                    while (bucket_pos != -1) {
                        bucket.load_from_file(data_file, bucket_pos, bucket_max_size); // Read bucket
                        // Search in bucket
                        for (auto &record :  bucket.records) {
                            if (record.get_key() >= begin_key && record.get_key() <= end_key) {
                                result_records.push_back(record);
                            }
                        }
                        #ifdef DEBUG_RANGESEARCH_STEP
                        cout << "[DEBUG]: Result in step (bucket_pos=" << bucket_pos << "):" <<endl;
                        for (auto &rec : result_records) {
                            cout << "[DEBUG]: -";
                            rec.print_data();
                            cout << endl;
                        }
                        #endif
                        bucket_pos = bucket.next_bucket; // Point to next bucket pos
                    }
                }
            }
            #ifdef DEBUG_ACTIONS
            else{
                cout << "[DEBUG]: rangeSearch (Fail open)" << endl;
            }
            #endif
            data_file.close();
            // Return
            #ifdef DEBUG_ACTIONS
            cout << "[DEBUG]: End rangeSearch" << endl;
            #endif
            return result_records;
        }
};


// #include "ExtensibleHash.h"

int main(int argc, char *argv[]) {
    filename_t filename  = "students_ehash";
    ExtensibleHash ehash(filename, 1024, 32);
    // GET ARGUMENTS
    if (argc != 3) {
        cout << "ERROR: INCORRECT ARGUMENTS FORMAT" << endl;
        cout << "./ehash {function} {parameter(s)}" << endl << endl;
        cout << "Functions      | Parameter(s)" << endl;
        cout << "- add            field1,field2,...,fieldn" << endl;
        cout << "- remove         key" << endl;
        cout << "- search         key" << endl;
        cout << "- rangeSearch    startkey,endkey" << endl;
        return 1;
    }
    std::string input_function = argv[1];
    std::string input_parameter = argv[2];

    if (input_function=="add") {
        Record record;
        record.set_data_from_string(input_parameter);
        bool result=ehash.add(record);
        //cout << result <<endl;
    } else if (input_function=="remove") {
        auto start = chrono::steady_clock::now();

        bool result = ehash.remove(input_parameter);

        auto end = chrono::steady_clock::now();
            
        cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
        cout << result <<endl;
    } else if (input_function=="search") {
        vector<Record> readdata;
        //cout << "Exact Search results (" <<input_parameter << "):" << endl;
        auto start = chrono::steady_clock::now();

        readdata = ehash.search(input_parameter);

        auto end = chrono::steady_clock::now();
            
        cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;
        if (readdata.size()){
            for (auto &rd : readdata) {
                rd.print_data();
                cout << endl;
            }
        }
    } else if (input_function=="rangeSearch") {
        stringstream ss(input_parameter);
        string start_key;
        string end_key;
        getline(ss, start_key, ',');
        getline(ss, end_key, ',');
        vector<Record> readdatarange;
        // NOTE: Por las operaciones si se tiene una subcadena, esta siempre se considera estrictamente menor
        //cout << "Range Search results (" << start_key << "," << end_key << "):" << endl;
        auto start = chrono::steady_clock::now();

        readdatarange = ehash.rangeSearch(start_key, end_key);
        auto end = chrono::steady_clock::now();
            
        if (readdatarange.size()){
            for (auto &rd : readdatarange) {
                rd.print_data();
                cout << endl;
            }
        }
        cout<<chrono::duration_cast<chrono::microseconds>(end - start).count()<<endl;

    } else {
        cout << "UNKNOWN FUNCTION" << endl;
    }


    
    /*
    filename_t filename  = "students_ehash";
    ExtensibleHash ehash(filename, 2, 3);
    // SAMPLE DATA:
    Record r1,r2,r3,r4,r5,r6;
    r1.set_data("Rodrigo","Salazar","11111","CS",19,6,120);
    r2.set_data("Luis","Ponce","11112","CS",20,7,130);
    r3.set_data("Diego","Guerra","11113","DS",21,8,140);
    r4.set_data("Arleth","Ivhy","11114","EE",22,9,150);
    r5.set_data("Juan","Sara","11115","EI",23,10,160);    
    r6.set_data("Eduardo","Arrozpide","11116","EQ",24,11,170);    
    // INSERT:
    vector<Record> sampledata {r1,r2,r3,r4,r5,r6};
    for (auto &r : sampledata) {
        cout << "Insert (" << r.get_key() << ") " << endl;
        ehash.add(r);
    }
    // READ EXACT:
    vector<Record> readdata;
    for (auto &r : sampledata) {
        cout << "Exact Search results (" << r.get_key() << "):" << endl;
        readdata = ehash.search(r.get_key());
        if (readdata.size()){
            for (auto &rd : readdata) {
                rd.print_data();
                cout << endl;
            }
        }
        else {
            cout << "NOT FOUND" << endl;
        }
    }
    // READ RANGE:
    vector<Record> readdatarange;
    // NOTE: Por las operaciones si se tiene una subcadena, esta siempre se considera estrictamente menor
    string start_key = "D";
    string end_key = "K";
    cout << "Range Search results (" << start_key << "," << end_key << "):" << endl;
    readdatarange = ehash.rangeSearch(start_key, end_key);
    if (readdatarange.size()){
        for (auto &rd : readdatarange) {
            rd.print_data();
            cout << endl;
        }
    }
    else {
        cout << "NOT FOUND" << endl;
    }
    // INSERT REPEATED
    cout << "INSERT REPEATED" <<endl;
    ehash.add(r1);
    ehash.add(r1);
    ehash.add(r1);    
    for (auto &r : sampledata) {
        cout << "Exact Search results (" << r.get_key() << "):" << endl;
        readdata = ehash.search(r.get_key());
        if (readdata.size()){
            for (auto &rd : readdata) {
                rd.print_data();
                cout << endl;
            }
        }
        else {
            cout << "NOT FOUND" << endl;
        }
    }
    // REMOVE
    cout << "REMOVE" <<endl;
    ehash.remove(r1.get_key());
    for (auto &r : sampledata) {
        cout << "Exact Search results (" << r.get_key() << "):" << endl;
        readdata = ehash.search(r.get_key());
        if (readdata.size()){
            for (auto &rd : readdata) {
                rd.print_data();
                cout << endl;
            }
        }
        else {
            cout << "NOT FOUND" << endl;
        }
    } 
    // REINSERT
    cout << "REINSERT" <<endl;
    ehash.add(r1);
    for (auto &r : sampledata) {
        cout << "Exact Search results (" << r.get_key() << "):" << endl;
        readdata = ehash.search(r.get_key());
        if (readdata.size()){
            for (auto &rd : readdata) {
                rd.print_data();
                cout << endl;
            }
        }
        else {
            cout << "NOT FOUND" << endl;
        }
    } 
        // INSERT REPEATED
    cout << "REINSERT REPEATED" <<endl;
    ehash.add(r1);
    ehash.add(r1);
    ehash.add(r1); 
    ehash.add(r1);
    ehash.add(r1);
    ehash.add(r1);    
    for (auto &r : sampledata) {
        cout << "Exact Search results (" << r.get_key() << "):" << endl;
        readdata = ehash.search(r.get_key());
        if (readdata.size()){
            for (auto &rd : readdata) {
                rd.print_data();
                cout << endl;
            }
        }
        else {
            cout << "NOT FOUND" << endl;
        }
    }
    */
}