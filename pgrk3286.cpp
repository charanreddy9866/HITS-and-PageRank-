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

// Function to initialize PageRank vector based on initial value
void initializePageRank(vector<double> &pageRank, int initialValue, int numVertices) {
    if (initialValue == 0) {
        fill(pageRank.begin(), pageRank.end(), 0);
    } else if (initialValue == 1) {
        fill(pageRank.begin(), pageRank.end(), 1.0);
    } else if (initialValue == -1) {
        fill(pageRank.begin(), pageRank.end(), 1.0 / numVertices);
    } else if (initialValue == -2) {
        double sqrtN = sqrt(numVertices);
        fill(pageRank.begin(), pageRank.end(), 1.0 / sqrtN);
    }
}

// Function to calculate and update PageRank values
void updatePageRank(vector<double> &pageRank, const vector<vector<int>> &incomingVertices,
                    const vector<int> &outDegree, double dampingFactor, double &error) {
    vector<double> newPageRank(pageRank.size());
    error = 0;

    for (size_t v = 0; v < pageRank.size(); ++v) {
        double sum = 0;

        // Calculate the sum of PageRank values from incoming vertices
        for (int incomingVertex : incomingVertices[v]) {
            sum += pageRank[incomingVertex] / outDegree[incomingVertex];
        }

        // Update PageRank values using the PageRank formula
        newPageRank[v] = (1 - dampingFactor) / pageRank.size() + dampingFactor * sum;

        // Update the error based on the difference between old and new PageRank values
        error = max(error, abs(newPageRank[v] - pageRank[v]));
    }

    // Update the PageRank vector
    pageRank = newPageRank;
}

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 4) {
        cout << "Usage: ./pgrk3286 iterations initialvalue somegraph.txt" << endl;
        return 1;
    }

    // Extract command-line arguments
    int iterations = stoi(argv[1]);
    int initialValue = stoi(argv[2]);
    double dampingFactor = 0.85; // Damping factor for PageRank
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

        // Create vectors to store PageRank values and out-degrees for each vertex
        vector<double> pageRank(numVertices);
        vector<int> outDegree(numVertices, 0);

        // Initialize PageRank values based on the specified initialValue
        initializePageRank(pageRank, initialValue, numVertices);

        // Set the error rate based on the specified iterations or error rate
        if (iterations == 0) {
            errorRate = pow(10, -5);
        } else if (iterations < 0) {
            errorRate = pow(10, iterations);
        }

        // Create vectors to store incoming vertices for each vertex
        vector<vector<int>> incomingVertices(numVertices);

        // Populate the incoming vertices and out-degrees based on edge information
        for (int i = 0; i < numEdges; i++) {
            getline(file, line);
            istringstream edge(line);
            int from, to;
            edge >> from >> to;
            incomingVertices[to].push_back(from);
            outDegree[from] += 1;
        }
        file.close();

        // Display initial values if the number of vertices is less than or equal to 10
        if (numVertices <= 10) {
            cout << "Base :0 :";
            for (int v = 0; v < numVertices; v++) {
                cout << "P[" << v << "]=" << fixed << setprecision(7) << pageRank[v] << "   ";
            }
            cout << endl;
        }

        int iteration;

        // Perform PageRank algorithm iterations
        for (iteration = 0; (iteration < iterations) || (iterations <= 0 && error > errorRate); iteration++) {
            updatePageRank(pageRank, incomingVertices, outDegree, dampingFactor, error);

            // Display values if the number of vertices is less than or equal to 10
            if (numVertices <= 10) {
                cout << "Iter :" << iteration + 1 << " :";
                for (int v = 0; v < numVertices; v++) {
                    cout << "P[" << v << "]=" << fixed << setprecision(7) << pageRank[v] << "   ";
                }
                cout << endl;
            }
        }

        // Display final values if the number of vertices is greater than 10
        if (numVertices > 10) {
            cout << "Iter :" << iteration << endl;
            for (int v = 0; v < numVertices; v++) {
                cout << "P[" << v << "]=" << fixed << setprecision(7) << pageRank[v] << endl;
            }
        }
    } catch (const ifstream::failure &e) {
        cerr << "Error in file reading: " << e.what() << endl;
        return 1;
    }

    return 0;
}
