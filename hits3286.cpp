//Charan Reddy
//Katta
//3286

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

// Function to initialize PageRank or HITS vectors based on initial value
void initializeVectors(vector<double> &values, int initialValue, int numVertices) {
    if (initialValue == 0) {
        fill(values.begin(), values.end(), 0);
    } else if (initialValue == 1) {
        fill(values.begin(), values.end(), 1.0);
    } else if (initialValue == -1) {
        fill(values.begin(), values.end(), 1.0 / numVertices);
    } else if (initialValue == -2) {
        double sqrtN = sqrt(numVertices);
        fill(values.begin(), values.end(), 1.0 / sqrtN);
    }
}

// Function to normalize vector values
void normalizeVector(vector<double> &values) {
    double sum = 0;
    for (double val : values) {
        sum += val * val;
    }

    double normFactor = sqrt(sum);
    for (double &val : values) {
        val /= normFactor;
    }
}

// Function to perform HITS algorithm iterations
void hitsAlgorithm(vector<vector<int>> &incomingEdges, vector<vector<int>> &outgoingEdges,
                   vector<double> &authority, vector<double> &hub, double &error) {
    vector<double> newAuthority(authority.size());
    vector<double> newHub(hub.size());
    error = 0;

    // Update authority values based on incoming edges
    for (size_t vertex = 0; vertex < authority.size(); ++vertex) {
        newAuthority[vertex] = 0;
        for (int incomingVertex : incomingEdges[vertex]) {
            newAuthority[vertex] += hub[incomingVertex];
        }
    }

    // Update hub values based on outgoing edges
    for (size_t vertex = 0; vertex < hub.size(); ++vertex) {
        newHub[vertex] = 0;
        for (int outgoingVertex : outgoingEdges[vertex]) {
            newHub[vertex] += newAuthority[outgoingVertex];
        }
    }

    // Normalize authority and hub values and calculate error
    normalizeVector(newAuthority);
    normalizeVector(newHub);

    for (size_t vertex = 0; vertex < authority.size(); ++vertex) {
        error = max(error, abs(newAuthority[vertex] - authority[vertex]));
        error = max(error, abs(newHub[vertex] - hub[vertex]));
    }

    // Update authority and hub vectors
    authority = newAuthority;
    hub = newHub;
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 4) {
        cout << "Usage: ./hits3286 iterations initialvalue somegraph.txt" << endl;
        return 1;
    }

    // Extract command-line arguments
    int iterations = stoi(argv[1]);
    int initialValue = stoi(argv[2]);
    double errorRate = 0;
    double error = 1;

    try {
        // Open the graph file
        ifstream file(argv[3]);
        string line;

        // Read the first line to get the number of vertices and edges in the graph
        getline(file, line);
        istringstream graphInfo(line);
        int numVertices, numEdges;
        graphInfo >> numVertices >> numEdges;

        // If the number of vertices is greater than 10, set iterations and initialValue accordingly
        if (numVertices > 10) {
            iterations = 0;
            initialValue = -1;
        }

        // Create adjacency lists for incoming and outgoing edges
        vector<vector<int>> incomingEdges(numVertices);
        vector<vector<int>> outgoingEdges(numVertices);

        // Populate the adjacency lists with edge information
        for (int i = 0; i < numEdges; i++) {
            getline(file, line);
            istringstream edge(line);
            int from, to;
            edge >> from >> to;
            incomingEdges[to].push_back(from);
            outgoingEdges[from].push_back(to);
        }
        file.close();

        // Create vectors to store authority and hub values for each vertex
        vector<double> authority(numVertices);
        vector<double> hub(numVertices);

        // Initialize authority and hub vectors based on the specified initialValue
        initializeVectors(authority, initialValue, numVertices);
        initializeVectors(hub, initialValue, numVertices);

        // Set the error rate based on the specified iterations or error rate
        if (iterations == 0) {
            errorRate = pow(10, -5);
        } else if (iterations < 0) {
            errorRate = pow(10, iterations);
        }

        // Display initial values if the number of vertices is less than or equal to 10
        if (numVertices <= 10) {
            cout << "Base :0 :";
            for (int vertex = 0; vertex < numVertices; vertex++) {
                cout << "A/H[ " << vertex << "]=" << fixed << setprecision(7) << authority[vertex] << "/" << fixed << setprecision(7) << hub[vertex] << " ";
            }
            cout << endl;
        }

        int iteration;

        // Perform HITS algorithm iterations
        for (iteration = 0; (iteration < iterations) || (iterations <= 0 && error > errorRate); iteration++) {
            hitsAlgorithm(incomingEdges, outgoingEdges, authority, hub, error);

            // Display values if the number of vertices is less than or equal to 10
            if (numVertices <= 10) {
                cout << "Iter : " << iteration + 1 << " : ";
                for (int vertex = 0; vertex < numVertices; vertex++) {
                    cout << "A/H[ " << vertex << "]=" << fixed << setprecision(7) << authority[vertex] << "/" << fixed << setprecision(7) << hub[vertex] << " ";
                }
                cout << endl;
            }
        }

        // Display final values if the number of vertices is greater than 10
        if (numVertices > 10) {
            cout << "Iter : " << iteration << endl;
            for (int vertex = 0; vertex < numVertices; vertex++) {
                cout << "A/H[ " << vertex << "]=" << fixed << setprecision(7) << authority[vertex] << "/" << fixed << setprecision(7) << hub[vertex] << endl;
            }
        }
    } catch (const ifstream::failure &e) {
        cerr << "Error in file reading: " << e.what() << endl;
        return 1;
    }

    return 0;
}
