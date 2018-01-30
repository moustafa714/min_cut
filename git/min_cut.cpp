//g++ min_cut.cpp -std=c++0x -o min_cut
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <random>
//#define RAND_ITRS    401234
#define RAND_ITRS    4
#define VERTICES_NUM 200
using namespace std;

class edge {
        public:
                int v1, v2; // two vertices connected by this edge
                edge() {
                        v1 = 0; // invalid
                        v2 = 0; // invalid
                }
                edge(int a, int b) {
                        v1 = a; 
                        v2 = b; 
                }
                void update(int a, int b) {
                        v1 = a; 
                        v2 = b; 
                }
                bool operator== (edge compared_edge) {
                        bool equal = (this->v1 == compared_edge.v1) &&
                                     (this->v2 == compared_edge.v2);
                        return equal;
                }
};

class node {
        public:
        edge node_edge;
        node * next = NULL;
};

class list {
        public:
                node * head;
                list(edge val) {
                        head              = new node;
                        head->node_edge   = val;
                        head->next        = NULL;
                }
                list() {
                        head              = NULL;
                }
                void insert(edge val) {
                        node * tail       = new node;
                        tail->node_edge   = val;

                        // uninitialized list
                        if (head == NULL) {
                                head                    = tail;
                                return;
                        }

                        // initialized list
                        node * curr_node  = head;
                        while (curr_node->next != NULL) {
                                curr_node               = curr_node->next;
                        }
                        curr_node->next   = tail;
                }
		void cp_to (list * l_dst) {
                        // uninitialized list
                        if (this->head == NULL) {
				l_dst->head = NULL;
                                return;
                        }

                        // initialized list
                        node * curr_node  = head;
       			l_dst->insert(curr_node->node_edge);
       			while (curr_node->next != NULL) {
				l_dst->insert(curr_node->next->node_edge);
                                curr_node               = curr_node->next;
                        }
			return;
		}
                int replace(edge req_edge, edge new_edge) {
                        int valid_replace = 0;
                        // uninitialized list
                        if (head == NULL) {
                                return valid_replace;
                        }

                        // initialized list
                        node * curr_node  = head;
                        // in case head is the req_edge
                        while (curr_node->node_edge == req_edge) {
                                curr_node->node_edge.v1 = new_edge.v1;
                                curr_node->node_edge.v2 = new_edge.v2;
                                valid_replace++;
                                curr_node               = curr_node->next;
                                if (curr_node == NULL) {
                                        return valid_replace;
                                }
                        }
                        // check the next nodes
                        node *  nxt_node  = curr_node->next;
                        while (nxt_node != NULL) {
                                if (nxt_node->node_edge == req_edge) {
                                        nxt_node->node_edge.v1 = new_edge.v1;
                                        nxt_node->node_edge.v2 = new_edge.v2;
                                        valid_replace++;
                                }
                                curr_node               = curr_node->next;
                                nxt_node                = nxt_node->next;
                        }
                        return valid_replace;
                }
                int count() {
                        int count         = 0;
                        //uninitialized list
                        if (head == NULL) {
                                return count;
                        }

                        // initialized list
                        node * curr_node  = head;
                        count++;
                        while (curr_node->next != NULL) {
                                count++;
                                curr_node               = curr_node->next;
                        }
                        return count;
                }
                void print() {
                        //uninitialized list
                        if (head == NULL) {
                                return;
                        }

                        // initialized list
                        node * curr_node  = head;
                        cout<<curr_node->node_edge.v1<<" => "<<curr_node->node_edge.v2<<endl;
                        while (curr_node->next != NULL) {
                                curr_node               = curr_node->next;
                                cout<<curr_node->node_edge.v1<<" => "<<curr_node->node_edge.v2<<endl;
                        }
                }
                void remove(edge req_edge) {
                        // uninitialized list
                        if (head == NULL) {
                                return;
                        }

                        // initialized list
                        node * curr_node  = head;
                        // in case head is the req_edge
                        while (curr_node->node_edge == req_edge) {
                                head                    = curr_node->next;
                                delete curr_node;
                                curr_node               = head;
                                if (curr_node == NULL) {
                                        return;
                                }
                        }
                        // check the next nodes
                        node *  nxt_node  = curr_node->next;
                        while (nxt_node != NULL) {
                                if (nxt_node->node_edge == req_edge) {
                                        curr_node->next = nxt_node->next;       
                                        delete nxt_node;
                                        nxt_node        = curr_node->next;
                                }
                                else {
                                        curr_node       = curr_node->next; // nxt_node
                                        nxt_node        = nxt_node->next;
                                }
                        }
                }
                int remove_one_edge() {
			int removed_edge_v2 = 0;
                        // uninitialized list
                        if (head == NULL) {
                                return removed_edge_v2;
                        }

                        // initialized list
			removed_edge_v2   = head->node_edge.v2;
                        node * curr_node  = head;
                        // in case head is the req_edge
                        head              = curr_node->next;
                        delete curr_node;
			return removed_edge_v2;
                }
                void delete_list() {
                        //uninitialized list
                        if (head == NULL) {
                                return;
                        }

                        // initialized list
                        node * curr_node  = head->next;
                        delete head;
                        while (curr_node != NULL) {
                                head                    = curr_node;
                                curr_node               = head->next;
                                delete head;
                        }
                        head              = NULL;
                }
                ~list() {
                        //uninitialized list
                        if (head == NULL) {
                                return;
                        }

                        // initialized list
                        node * curr_node  = head->next;
                        delete head;
                        while (curr_node != NULL) {
                                head                    = curr_node;
                                curr_node               = head->next;
                                delete head;
                        }
                        head              = NULL;
                }
};

class vertex {
        public:
                list edges;
                edge e;
                edge new_edge;
                vertex() {
                }
                void insert_edge(int a, int b) {
                        e.update(a, b);
                        edges.insert(e);
                } 
                void cp_to(vertex * dst_v) {
                        this->edges.cp_to(&dst_v->edges);
                } 
                void remove_edge(int a, int b) {
                        e.update(a, b);
                        edges.remove(e);
                } 
                int replace_edge(int a, int b, int new_a, int new_b) {
                        e.update(a, b);
                        new_edge.update(new_a, new_b);
                        int valid_replace = edges.replace(e, new_edge);
                        return valid_replace;
                } 
                void print() {
                        if (edges.head == NULL) {
                                return;
                        }
                        edges.print();
                        cout<<endl;
                } 
                list get_edges() {
                        return edges;
                } 
                void delete_edges() {
                        edges.delete_list();
                } 
                int count() {
                        return edges.count();
                }
                int remove_one_edge() {
                        return edges.remove_one_edge();
                }
//		vertex operator= (vertex v_to_cp) {
//			this->edges = v_to_cp.edges;
//			return *this;
//		}
};

int main() {
        vertex v[VERTICES_NUM + 1];
        vertex v_backup[VERTICES_NUM + 1];
        int i;
        bool valid_vertices[VERTICES_NUM + 1];
        int * min_cuts = new int[RAND_ITRS];
        int * s        = new int[RAND_ITRS];
        int * d        = new int[RAND_ITRS];

        // get the vertices
        for (i = 1; i <= VERTICES_NUM; i++) {
                valid_vertices[i] = 1;
        }
        
        ifstream numbers;
        numbers.open("kargerMinCut.txt"); 
        i = 1;
        string line;
        while (getline(numbers, line)) {
                stringstream linestream(line);
                int v1 = 0;
                int v2 = 0;
                while (linestream>>v2) {
                        if (v1 == 0) {
                                v1 = v2;
                        }
                        else {
                                v_backup[i].insert_edge(v1, v2);
                        }
                }
                i++;
        }



        int rand_itr;
        int v1, v2, itr;
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 eng(rd()); // seed the generator
        std::uniform_int_distribution<> distr(1, VERTICES_NUM); // define the range

        for (rand_itr = 0; rand_itr < RAND_ITRS; rand_itr++) {
		// prepare the vertices
		for (i = 1; i <= VERTICES_NUM; i++) {
			v_backup[i].cp_to(&v[i]);
			valid_vertices[i] = 1;
		}

                for (itr = 1; itr <= VERTICES_NUM - 2; itr++) {
                        i  = 0;
                        v1 = 0;
                        v2 = 0;
                        // pick a random edge
                        while (!(valid_vertices[v1])) {
                                v1 = distr(eng);
//				v1 = rand() % VERTICES_NUM + 1;
                        }
                        while (!(valid_vertices[v2] && (v1 != v2))) {
                                v2 = distr(eng);
//       				v2 = rand() % VERTICES_NUM + 1;
	                }
                //        cout<<itr<<"Edge to remove: v1 = "<<v1<<", v2 = "<<v2<<endl;
                        v[v2].remove_edge(v2, v1);
                        v[v1].remove_edge(v1, v2);
                        valid_vertices[v1]  = 0;
			int removed_edge_v = v[v1].remove_one_edge();
			while (removed_edge_v) {
				v[removed_edge_v].replace_edge(removed_edge_v, v1, removed_edge_v, v2);
				v[v2].insert_edge(v2, removed_edge_v);
				removed_edge_v = v[v1].remove_one_edge();
			};
                        v[v1].delete_edges();
                }
        //      cout<<"After replace:\n";
                v1 = 0;
                v2 = 0; 
//		int valid_vertices_count = 0;
                for (i = 1; i <= VERTICES_NUM; i++) {
                        if (valid_vertices[i]) {
                                if (!v1) {
                                        v1 = i;
                                }
                                else if (!v2) {
                                        v2 = i;
					break;
                                }
//				else {
//					cout<<"ERROR: There should be two vertices only at the end\n";
//					exit(1);
//				}
//				valid_vertices_count++;
                        }
                }
                int v1_count = v[v1].count();
                int v2_count = v[v2].count();
//                cout<<rand_itr<<"::v1 = "<<v1<<", v1 count: "<<v1_count<<", v2 = "<<v2<<", v2 count: "<<v2_count<<endl;
//                cout<<"valid_vertices_count = "<<valid_vertices_count<<endl;
                v[v1].delete_edges();
                v[v2].delete_edges();
                min_cuts[rand_itr] = v1_count;
		s[rand_itr]        = v1;
		d[rand_itr]        = v2;
	}
      cout<<"==================== min_cuts ====================="<<endl;
        int min_cut = 999999;
	int best_itr;
        for (rand_itr = 0; rand_itr < RAND_ITRS; rand_itr++) {
                if (min_cuts[rand_itr] < min_cut) {
                        min_cut  = min_cuts[rand_itr]; 
			best_itr = rand_itr;
                }
        }
                cout<<"s = "<<s[best_itr]<<", d = "<<d[best_itr]<<", min_cut = "<<min_cuts[best_itr]<<endl;
        delete [] min_cuts;
        delete [] s;
        delete [] d;
	for (i = 1; i < VERTICES_NUM; i++) {
		v_backup[i].delete_edges();
	}
        return 0;
}

//1 7 5 3 66 88 55
//2 44 33 11
//      v[0].insert_edge(1, 7);
//      v[0].insert_edge(1, 5);
//      v[0].insert_edge(1, 7);
//      v[0].insert_edge(1, 3);
//      v[0].insert_edge(1, 7);
//
//      v[0].print();
//
//      v[0].remove_edge(1, 7);
//      v[0].print();

//      v[0].insert_edge(1, 7);
//      v[0].insert_edge(1, 5);
//      v[0].insert_edge(1, 7);
//      v[0].insert_edge(1, 3);
//      v[0].insert_edge(1, 7);
//
//      v[0].print();
//
//      v[0].replace_edge(1, 7, 1, 20);
//      v[0].print();



