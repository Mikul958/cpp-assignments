// Copyright 2023 mpikula

# include <iostream>
# include <iomanip>

using std::cout;
using std::cin;
using std::endl;
using std::setfill;
using std::setw;

bool charIsValid(char character) {
    return character != ':' && ((character - '0') < 10);
}

int main() {
    char char1, char2, char3, char4, char5;
    int mm, ss, seconds, lowest_seconds, total_seconds, count;
    lowest_seconds = total_seconds = 0;
    count = 1;
    bool valid = true;

    // Take user inputs and determine fastest time and total time in seconds.
    while (true) {
        // Read in first character and check to see if it is q.
        cin >> char1;
        if (char1 == 'q') {
            break;
        }

        // Read in more characters and check to see if 3rd is a :.
        // Clear input buffer, set to invalid, and skip rest of loop if no.
        cin >> char2 >> char3;
        if (char3 != ':') {
            valid = false;
            cin.ignore(100, '\n');   // Clear input buffer.
            continue;
        }

        // Read in remaining characters from user input.
        cin >> char4;
        cin >> char5;

        // If invalid input was already entered, ignore the rest.
        if (valid) {
            // Convert char inputs to integers and obtain minutes and seconds.
            mm = (char1 - '0')*10 + (char2 - '0');
            ss = (char4 - '0')*10 + (char5 - '0');
            // Determine if entered times have valid values.
            if (mm >= 60 || ss >= 60)  {
                valid = false;
                continue;
            }
            // Convert obtained time to seconds and add to total.
            seconds = mm*60 + ss;
            total_seconds += seconds;
            // Check for new lowest time (will always work if first loop).
            if (count == 1 || seconds < lowest_seconds) {
                lowest_seconds = seconds;
            }
            count++;
        }
    }

    // Convert lowest/total times from seconds back into (hh:)mm:ss format.
    int lowest_ss, lowest_mm, total_ss, total_mm, total_hh;
    total_hh = 0;
    lowest_ss = lowest_seconds % 60;
    lowest_mm = (lowest_seconds - lowest_ss) / 60;
    total_ss = total_seconds % 60;
    total_mm = (total_seconds - total_ss) / 60;
    if (total_mm >= 60) {
        int temp = total_mm % 60;
        total_hh = (total_mm - temp) / 60;   // Obtain hours from minutes.
        total_mm = temp;   // Set minutes equal to remainder of minutes / 60.
    }

    // Print results
    if (valid) {
        if (count-1 != 1) {
            cout << count-1 << " valid times read." << endl;
        } else {
            cout << count-1 << " valid time read." << endl;
        }
    } else {
        cout << "Input " << count << " did not match expected format." << endl
        << "Fastest and total running times are based on the first "
        << count-1 << " valid times read." << endl;
    }
    cout << "Fastest mile time: " << setfill('0') << setw(2) << lowest_mm
    << ':' << setw(2) << lowest_ss << endl;
    if (total_hh != 0) {
        cout << "Total running time: " << total_hh << ':'
        << setw(2) << total_mm << ':' << setw(2) << total_ss << endl;
    } else {
        cout << "Total running time: " << setw(2) << total_mm << ':'
        << setw(2) << total_ss << endl;
    }

    return 0;
}
