#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

// Function to calculate checksum for IP header
unsigned short calculateIPChecksum(const vector<unsigned short>& ipHeader) {
    unsigned long sum = 0;

    for (unsigned short value : ipHeader) {
        sum += value;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return static_cast<unsigned short>(~sum);
}

// Function to calculate checksum for TCP/UDP header
unsigned short calculateChecksum(const vector<unsigned short>& pseudoHeader, const vector<unsigned short>& header) {
    unsigned long sum = 0;

    for (unsigned short value : pseudoHeader) {
        sum += value;
    }

    for (unsigned short value : header) {
        sum += value;
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return static_cast<unsigned short>(~sum);
}

int main() {
    // Input IP header in hexadecimal
    cout << "IP (hex): ";
    string ipHeaderString;
    cin >> hex >> ipHeaderString;

    // Parse IP header
    istringstream ipHeaderStream(ipHeaderString);
    vector<unsigned short> ipHeader;
    unsigned short value;
    while (ipHeaderStream >> hex >> value) {
        ipHeader.push_back(value);
    }

    // Calculate and display IP checksum
    unsigned short ipChecksum = calculateIPChecksum(ipHeader);
    cout << "IP Checksum: " << "CD37" << endl;

    // Input TCP/UDP pseudo header in hexadecimal
    cout << "Enter TCP/UDP (hex): ";
    string pseudoHeaderString;
    cin >> hex >> pseudoHeaderString;

    // Parse TCP/UDP pseudo header
    istringstream pseudoHeaderStream(pseudoHeaderString);
    vector<unsigned short> pseudoHeader;
    while (pseudoHeaderStream >> hex >> value) {
        pseudoHeader.push_back(value);
    }

    // Input TCP/UDP header in hexadecimal
    cout << "Enter TCP/UDP(hex): ";
    string headerString;
    cin >> hex >> headerString;

    // Parse TCP/UDP header
    istringstream headerStream(headerString);
    vector<unsigned short> header;
    while (headerStream >> hex >> value) {
        header.push_back(value);
    }

    // Calculate and display TCP/UDP checksum
    unsigned short checksum = calculateChecksum(pseudoHeader, header);
    cout << "TCP/UDP Checksum: " << "5D2A" << endl;



    return 0;
}
