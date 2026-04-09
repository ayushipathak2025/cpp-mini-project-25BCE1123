#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

// ==================== ROUTE CLASS ====================
/**
 * @class Route
 * @brief Manages bus route information and seat availability.
 *
 * This class encapsulates route details (RouteID, From, To, Fare, Capacity)
 * and maintains seat availability using a std::vector<bool>.
 * Provides methods to check seat availability and book/release seats.
 */
class Route
{
private:
    string routeID;          // Unique identifier for the route
    string from;             // Departure city
    string to;               // Destination city
    double fare;             // Cost per ticket for this route
    int seatCapacity;        // Total number of seats available
    vector<bool> seatStatus; // true = booked, false = available

public:
    // Constructor
    /**
     * @brief Initializes a Route with given parameters.
     * @param id Route identifier
     * @param fromCity Departure city
     * @param toCity Destination city
     * @param ticketFare Cost per ticket
     * @param capacity Total seat capacity
     */
    Route(string id, string fromCity, string toCity, double ticketFare, int capacity)
        : routeID(id), from(fromCity), to(toCity), fare(ticketFare), seatCapacity(capacity)
    {
        // Initialize all seats as available (false = not booked)
        seatStatus.assign(capacity, false);
    }

    // Default constructor
    Route() : routeID(""), from(""), to(""), fare(0.0), seatCapacity(0) {}

    // Getters (accessors)
    string getRouteID() const { return routeID; }
    string getFrom() const { return from; }
    string getTo() const { return to; }
    double getFare() const { return fare; }
    int getSeatCapacity() const { return seatCapacity; }

    /**
     * @brief Returns the number of available (unbooked) seats.
     */
    int getAvailableSeats() const
    {
        return seatCapacity - count(seatStatus.begin(), seatStatus.end(), true);
    }

    /**
     * @brief Checks if a specific seat is available.
     * @param seatNumber The seat to check (1-based indexing)
     * @return true if available, false if booked or invalid
     */
    bool isSeatAvailable(int seatNumber) const
    {
        // Convert to 0-based indexing for vector
        int index = seatNumber - 1;
        if (index < 0 || index >= seatCapacity)
        {
            return false; // Invalid seat number
        }
        return !seatStatus[index]; // Return true if not booked
    }

    /**
     * @brief Books a seat on this route.
     * @param seatNumber The seat to book (1-based)
     * @return true if booking successful, false if already booked or invalid
     */
    bool bookSeat(int seatNumber)
    {
        int index = seatNumber - 1;
        if (index < 0 || index >= seatCapacity || seatStatus[index])
        {
            return false; // Invalid or already booked
        }
        seatStatus[index] = true; // Mark as booked
        return true;
    }

    /**
     * @brief Releases a booked seat.
     * @param seatNumber The seat to release (1-based)
     * @return true if release successful, false if not booked or invalid
     */
    bool releaseSeat(int seatNumber)
    {
        int index = seatNumber - 1;
        if (index < 0 || index >= seatCapacity || !seatStatus[index])
        {
            return false; // Invalid or not booked
        }
        seatStatus[index] = false; // Mark as available
        return true;
    }

    /**
     * @brief Returns a string representation of seat status.
     * Format: "Route1,Delhi,Mumbai,500.00,50,S1,S3,S5,..." (booked seats)
     */
    string serialize() const
    {
        ostringstream oss;
        oss << routeID << "," << from << "," << to << "," << fixed << setprecision(2) << fare << "," << seatCapacity;

        // Append booked seat numbers
        for (int i = 0; i < seatCapacity; ++i)
        {
            if (seatStatus[i])
            {
                oss << ",S" << (i + 1); // Seat numbers start from 1
            }
        }
        return oss.str();
    }

    /**
     * @brief Returns the seat type (Window, Middle-Left, Middle-Right).
     * Bus layout: 4 seats per row (positions 1-4 in each row)
     * - Position 1, 4: Window seat
     * - Position 2: Middle-Left
     * - Position 3: Middle-Right
     * @param seatNumber The seat (1-based indexing)
     * @return String describing the seat type and position
     */
    string getSeatType(int seatNumber) const
    {
        if (seatNumber < 1 || seatNumber > seatCapacity)
            return "Invalid";

        int positionInRow = ((seatNumber - 1) % 4) + 1;
        if (positionInRow == 1 || positionInRow == 4)
            return "WINDOW";
        else if (positionInRow == 2)
            return "MIDDLE-LEFT";
        else
            return "MIDDLE-RIGHT";
    }

    /**
     * @brief Displays route information in a formatted manner.
     */
    void displayInfo() const
    {
        cout << "  Route ID: " << routeID << " | From: " << from << " | To: " << to
             << " | Fare: Rs. " << fixed << setprecision(2) << fare
             << " | Available Seats: " << getAvailableSeats() << "/" << seatCapacity << endl;
    }

    /**
     * @brief Displays all seats with their availability and type for booking.
     */
    void displaySeatsForBooking() const
    {
        cout << "\n============================================================" << endl;
        cout << "                    AVAILABLE SEAT LAYOUT                    " << endl;
        cout << "============================================================" << endl;
        cout << "\nRoute: " << from << " --> " << to << " | Fare: Rs. " << fixed << setprecision(2) << fare << endl;
        cout << "Seat Capacity: " << seatCapacity << " | Available: " << getAvailableSeats() << endl;
        cout << "\n[A] = AVAILABLE | [X] = BOOKED" << endl;
        cout << "\n";

        // Display seats in rows of 4
        for (int i = 1; i <= seatCapacity; ++i)
        {
            int posInRow = ((i - 1) % 4) + 1;
            string status = seatStatus[i - 1] ? "[X]" : "[A]";
            string seatType;

            if (posInRow == 1 || posInRow == 4)
                seatType = "WINDOW";
            else if (posInRow == 2)
                seatType = "MIDDLE-L";
            else
                seatType = "MIDDLE-R";

            cout << "Seat " << setw(2) << i << " (" << setw(8) << seatType << ") " << status << "  ";

            // New line after every 2 seats for better formatting
            if (i % 2 == 0)
                cout << endl;
        }
        cout << endl;
        cout << "============================================================" << endl;
    }
};

// ==================== TICKET CLASS ====================
/**
 * @class Ticket
 * @brief Manages individual ticket information.
 *
 * Each ticket stores passenger details, booked route, seat number,
 * and amount paid. TicketID is auto-generated and unique.
 */
class Ticket
{
private:
    string ticketID;      // Unique ticket identifier
    string passengerName; // Name of the passenger
    string routeID;       // Associated route
    int bookedSeat;       // Booked seat number (1-based)
    double amountPaid;    // Total amount paid by passenger

public:
    // Constructor
    /**
     * @brief Initializes a Ticket with booking details.
     * @param id Unique ticket identifier
     * @param name Passenger name
     * @param route Associated route ID
     * @param seat Booked seat number
     * @param amount Amount paid
     */
    Ticket(string id, string name, string route, int seat, double amount)
        : ticketID(id), passengerName(name), routeID(route), bookedSeat(seat), amountPaid(amount) {}

    // Default constructor
    Ticket() : ticketID(""), passengerName(""), routeID(""), bookedSeat(0), amountPaid(0.0) {}

    // Getters
    string getTicketID() const { return ticketID; }
    string getPassengerName() const { return passengerName; }
    string getRouteID() const { return routeID; }
    int getBookedSeat() const { return bookedSeat; }
    double getAmountPaid() const { return amountPaid; }

    /**
     * @brief Returns a string representation for file storage.
     * Format: "TK001,John,Route1,15,500.00"
     */
    string serialize() const
    {
        ostringstream oss;
        oss << ticketID << "," << passengerName << "," << routeID << ","
            << bookedSeat << "," << fixed << setprecision(2) << amountPaid;
        return oss.str();
    }

    /**
     * @brief Displays ticket information.
     */
    void displayInfo() const
    {
        cout << "  Ticket ID: " << ticketID << " | Passenger: " << passengerName
             << " | Route: " << routeID << " | Seat: " << bookedSeat
             << " | Amount: Rs. " << fixed << setprecision(2) << amountPaid << endl;
    }
};

// ==================== RESERVATION SYSTEM CLASS ====================
/**
 * @class ReservationSystem
 * @brief Main system controller managing routes, tickets, and file I/O.
 *
 * This class orchestrates all operations: route management, ticket booking/cancellation,
 * searching, and report generation. It also handles persistence via file I/O.
 */
class ReservationSystem
{
private:
    vector<Route> routes;            // Collection of all routes
    vector<Ticket> tickets;          // Collection of all booked tickets
    map<string, int> ticketQuantity; // Map: routeID -> ticket count per route
    string dataFile;                 // File for persistent storage
    int ticketCounter;               // Counter for auto-generating ticket IDs

    /**
     * @brief Generates a unique ticket ID.
     * Format: "TK001", "TK002", etc.
     * @return A new unique ticket ID
     */
    string generateTicketID()
    {
        ticketCounter++;
        ostringstream oss;
        oss << "TK" << setfill('0') << setw(3) << ticketCounter;
        return oss.str();
    }

    /**
     * @brief Finds a route by its ID.
     * @param routeID The route to find
     * @return Pointer to the route, nullptr if not found
     */
    Route *findRoute(const string &routeID)
    {
        for (auto &route : routes)
        {
            if (route.getRouteID() == routeID)
            {
                return &route;
            }
        }
        return nullptr;
    }

    /**
     * @brief Finds a ticket by its ID.
     * @param ticketID The ticket to find
     * @return Iterator to the ticket, end() if not found
     */
    vector<Ticket>::iterator findTicket(const string &ticketID)
    {
        for (auto it = tickets.begin(); it != tickets.end(); ++it)
        {
            if (it->getTicketID() == ticketID)
            {
                return it;
            }
        }
        return tickets.end();
    }

public:
    // Constructor
    /**
     * @brief Initializes the reservation system with data file path.
     * @param fileName Path to the data file for persistence
     */
    ReservationSystem(string fileName = "tickets.txt")
        : dataFile(fileName), ticketCounter(0)
    {
        // Initialize predefined routes
        routes.push_back(Route("Route1", "Delhi", "Mumbai", 500.00, 50));
        routes.push_back(Route("Route2", "Bangalore", "Chennai", 300.00, 40));
        routes.push_back(Route("Route3", "Kolkata", "Delhi", 400.00, 35));
        routes.push_back(Route("Route4", "Pune", "Goa", 250.00, 30));

        // Load existing data from file
        loadData();
    }

    /**
     * @brief Saves all routes and tickets to a file.
     * Format: One entry per line (either route or ticket data).
     */
    void saveData()
    {
        ofstream file(dataFile);
        if (!file.is_open())
        {
            cerr << "Error: Could not open file for saving!" << endl;
            return;
        }

        // Save routes
        file << "[ROUTES]" << endl;
        for (const auto &route : routes)
        {
            file << route.serialize() << endl;
        }

        // Save tickets
        file << "[TICKETS]" << endl;
        for (const auto &ticket : tickets)
        {
            file << ticket.serialize() << endl;
        }

        file.close();
        cout << "Data saved successfully!" << endl;
    }

    /**
     * @brief Loads routes and tickets from file.
     * Reconstructs the system state after restart.
     */
    void loadData()
    {
        ifstream file(dataFile);
        if (!file.is_open())
        {
            // File doesn't exist yet; start fresh
            return;
        }

        string line;
        bool loadingRoutes = false;
        bool loadingTickets = false;

        while (getline(file, line))
        {
            if (line == "[ROUTES]")
            {
                loadingRoutes = true;
                loadingTickets = false;
                continue;
            }
            else if (line == "[TICKETS]")
            {
                loadingRoutes = false;
                loadingTickets = true;
                continue;
            }

            if (line.empty())
                continue;

            if (loadingRoutes)
            {
                // Parse route line: "Route1,Delhi,Mumbai,500.00,50,S1,S3,..."
                istringstream iss(line);
                string routeID, from, to, fareStr, capacityStr;

                getline(iss, routeID, ',');
                getline(iss, from, ',');
                getline(iss, to, ',');
                getline(iss, fareStr, ',');
                getline(iss, capacityStr, ',');

                double fare = stod(fareStr);
                int capacity = stoi(capacityStr);

                // Find and update the existing route
                Route *route = findRoute(routeID);
                if (route)
                {
                    string seatStr;
                    while (getline(iss, seatStr, ','))
                    {
                        if (!seatStr.empty() && seatStr[0] == 'S')
                        {
                            int seatNum = stoi(seatStr.substr(1));
                            route->bookSeat(seatNum);
                        }
                    }
                }
            }
            else if (loadingTickets)
            {
                // Parse ticket line: "TK001,John,Route1,15,500.00"
                istringstream iss(line);
                string ticketID, passengerName, routeID, seatStr, amountStr;

                getline(iss, ticketID, ',');
                getline(iss, passengerName, ',');
                getline(iss, routeID, ',');
                getline(iss, seatStr, ',');
                getline(iss, amountStr, ',');

                int seat = stoi(seatStr);
                double amount = stod(amountStr);

                tickets.push_back(Ticket(ticketID, passengerName, routeID, seat, amount));
                ticketQuantity[routeID]++;

                // Extract ticket counter from ID (e.g., "TK001" -> 1)
                int num = stoi(ticketID.substr(2));
                if (num > ticketCounter)
                {
                    ticketCounter = num;
                }
            }
        }

        file.close();
    }

    /**
     * @brief Displays all available routes.
     */
    void displayRoutes()
    {
        cout << "\n============================================================" << endl;
        cout << "                     AVAILABLE ROUTES                      " << endl;
        cout << "============================================================" << endl;
        if (routes.empty())
        {
            cout << "No routes available." << endl;
            cout << "============================================================" << endl
                 << endl;
            return;
        }

        for (size_t i = 0; i < routes.size(); ++i)
        {
            cout << "\n[" << (i + 1) << "] ";
            routes[i].displayInfo();
        }
        cout << "\n============================================================" << endl
             << endl;
    }

    /**
     * @brief Books a ticket for a passenger on a specified route and seat.
     * @return true if booking successful, false otherwise
     */
    bool bookTicket()
    {
        cout << "\n============================================================" << endl;
        cout << "                       BOOK TICKET                          " << endl;
        cout << "============================================================" << endl;

        // Validate that routes exist
        if (routes.empty())
        {
            cerr << "Error: No routes available!" << endl;
            return false;
        }

        displayRoutes();

        // Input: Route selection
        cout << "Enter route choice number (1-" << routes.size() << "): ";
        cout.flush(); // Ensure prompt is displayed immediately
        int routeChoice;
        if (!(cin >> routeChoice) || routeChoice < 1 || routeChoice > (int)routes.size())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cerr << "Invalid route choice!" << endl;
            return false;
        }
        cin.ignore(10000, '\n'); // Clear entire line after reading number

        Route &selectedRoute = routes[routeChoice - 1];

        // Display detailed seat layout with types and availability
        selectedRoute.displaySeatsForBooking();

        // Input: Passenger name
        cout << "\nEnter passenger name: ";
        cout.flush(); // Ensure prompt is displayed immediately
        string passengerName;
        if (!getline(cin, passengerName) || passengerName.empty())
        {
            cerr << "Invalid passenger name!" << endl;
            return false;
        }

        // Input: Seat number
        cout << "\nEnter seat number (1-" << selectedRoute.getSeatCapacity() << "): ";
        cout.flush(); // Ensure prompt is displayed immediately
        int seatNumber;
        if (!(cin >> seatNumber))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cerr << "Invalid seat number!" << endl;
            return false;
        }
        cin.ignore(10000, '\n'); // Clear entire line after reading number

        // Validate seat number range
        if (seatNumber < 1 || seatNumber > selectedRoute.getSeatCapacity())
        {
            cerr << "Error: Seat number must be between 1 and " << selectedRoute.getSeatCapacity() << "!" << endl;
            return false;
        }

        // Validate seat availability
        if (!selectedRoute.isSeatAvailable(seatNumber))
        {
            cerr << "Error: Seat " << seatNumber << " is already booked!" << endl;
            return false;
        }

        // Book the seat
        if (!selectedRoute.bookSeat(seatNumber))
        {
            cerr << "Error: Could not book the seat!" << endl;
            return false;
        }

        // Create and store ticket
        string ticketID = generateTicketID();
        double fare = selectedRoute.getFare();
        Ticket newTicket(ticketID, passengerName, selectedRoute.getRouteID(), seatNumber, fare);
        tickets.push_back(newTicket);
        ticketQuantity[selectedRoute.getRouteID()]++;

        // Display booking confirmation
        cout << "\n============================================================" << endl;
        cout << "                  BOOKING SUCCESSFUL!                       " << endl;
        cout << "============================================================" << endl;
        cout << "\n  TICKET DETAILS:" << endl;
        cout << "  " << string(56, '-') << endl;
        cout << "  Ticket ID      : " << ticketID << endl;
        cout << "  Passenger Name : " << passengerName << endl;
        cout << "  Route          : " << selectedRoute.getFrom() << " --> " << selectedRoute.getTo() << endl;
        cout << "  Seat Number    : " << seatNumber << endl;
        cout << "  Seat Type      : " << selectedRoute.getSeatType(seatNumber) << endl;
        cout << "  Amount Paid    : Rs. " << fixed << setprecision(2) << fare << endl;
        cout << "  " << string(56, '-') << endl;
        cout << "\n============================================================" << endl
             << endl;

        return true;
    }

    /**
     * @brief Cancels a ticket and releases its reserved seat.
     * @return true if cancellation successful, false otherwise
     */
    bool cancelTicket()
    {
        cout << "\n============================================================" << endl;
        cout << "                    CANCEL TICKET                           " << endl;
        cout << "============================================================" << endl;

        cout << "\nEnter Ticket ID to cancel: ";
        cout.flush(); // Ensure prompt is displayed
        string ticketID;
        if (!getline(cin, ticketID) || ticketID.empty())
        {
            cerr << "\nError: Invalid Ticket ID!" << endl;
            return false;
        }

        // Trim whitespace
        while (!ticketID.empty() && isspace(ticketID.back()))
            ticketID.pop_back();
        while (!ticketID.empty() && isspace(ticketID.front()))
            ticketID.erase(0, 1);

        // Find the ticket
        auto ticketIt = findTicket(ticketID);
        if (ticketIt == tickets.end())
        {
            cerr << "\nError: Ticket " << ticketID << " not found!" << endl;
            return false;
        }

        // Store ticket details before erasing
        string passengerName = ticketIt->getPassengerName();
        string routeID = ticketIt->getRouteID();
        int bookedSeat = ticketIt->getBookedSeat();
        double amountPaid = ticketIt->getAmountPaid();

        // Find the associated route and release the seat
        Route *route = findRoute(routeID);
        if (route)
        {
            if (!route->releaseSeat(bookedSeat))
            {
                cerr << "\nError: Could not release the seat!" << endl;
                return false;
            }
        }

        // Remove the ticket
        ticketQuantity[routeID]--;
        tickets.erase(ticketIt);

        // Display cancellation confirmation
        cout << "\n============================================================" << endl;
        cout << "                CANCELLATION SUCCESSFUL!                    " << endl;
        cout << "============================================================" << endl;
        cout << "\n  REFUND DETAILS:" << endl;
        cout << "  " << string(56, '-') << endl;
        cout << "  Ticket ID      : " << ticketID << endl;
        cout << "  Passenger Name : " << passengerName << endl;
        cout << "  Route ID       : " << routeID << endl;
        cout << "  Seat Released  : " << bookedSeat << endl;
        cout << "  Refund Amount  : Rs. " << fixed << setprecision(2) << amountPaid << endl;
        cout << "  " << string(56, '-') << endl;
        cout << "\n============================================================" << endl
             << endl;

        return true;
    }

    /**
     * @brief Searches for and displays a specific ticket by ID.
     */
    void searchTicket()
    {
        cout << "\n============================================================" << endl;
        cout << "                    SEARCH TICKET                           " << endl;
        cout << "============================================================" << endl;

        cout << "\nEnter Ticket ID to search: ";
        cout.flush(); // Ensure prompt is displayed
        string ticketID;
        if (!getline(cin, ticketID) || ticketID.empty())
        {
            cerr << "\nError: Invalid Ticket ID!" << endl;
            return;
        }

        // Trim whitespace
        while (!ticketID.empty() && isspace(ticketID.back()))
            ticketID.pop_back();
        while (!ticketID.empty() && isspace(ticketID.front()))
            ticketID.erase(0, 1);

        auto ticketIt = findTicket(ticketID);
        if (ticketIt == tickets.end())
        {
            cerr << "\nError: Ticket " << ticketID << " not found!" << endl;
            return;
        }

        cout << "\n============================================================" << endl;
        cout << "                      TICKET DETAILS                        " << endl;
        cout << "============================================================" << endl;
        ticketIt->displayInfo();
        cout << "============================================================" << endl
             << endl;
    }

    /**
     * @brief Generates and displays various reports:
     * - Revenue per route
     * - Total tickets booked per route
     * - Identification of the most popular route
     */
    void generateReports()
    {
        cout << "\n========== RESERVATION REPORTS ==========\n";

        if (tickets.empty())
        {
            cout << "No bookings found." << endl;
            cout << "=========================================\n"
                 << endl;
            return;
        }

        cout << "\n[1] REVENUE PER ROUTE\n";
        cout << "---------------------\n";
        map<string, double> revenuePerRoute;
        for (const auto &ticket : tickets)
        {
            revenuePerRoute[ticket.getRouteID()] += ticket.getAmountPaid();
        }

        double totalRevenue = 0.0;
        for (const auto &revenue : revenuePerRoute)
        {
            cout << "  " << revenue.first << ": Rs. " << fixed << setprecision(2) << revenue.second << endl;
            totalRevenue += revenue.second;
        }
        cout << "  TOTAL REVENUE: Rs. " << fixed << setprecision(2) << totalRevenue << endl;

        cout << "\n[2] TICKETS BOOKED PER ROUTE\n";
        cout << "----------------------------\n";
        for (const auto &count : ticketQuantity)
        {
            cout << "  " << count.first << ": " << count.second << " ticket(s)" << endl;
        }

        cout << "\n[3] MOST POPULAR ROUTE\n";
        cout << "---------------------\n";
        if (!ticketQuantity.empty())
        {
            string mostPopular = max_element(ticketQuantity.begin(), ticketQuantity.end(),
                                             [](const pair<string, int> &a, const pair<string, int> &b)
                                             {
                                                 return a.second < b.second;
                                             })
                                     ->first;
            cout << "  Most Popular Route: " << mostPopular << " (" << ticketQuantity[mostPopular] << " bookings)" << endl;
        }

        cout << "\n=========================================\n"
             << endl;
    }

    /**
     * @brief Displays current system statistics.
     */
    void displayStatistics()
    {
        cout << "\n========== SYSTEM STATISTICS ==========\n";
        cout << "  Total Routes: " << routes.size() << endl;
        cout << "  Total Bookings: " << tickets.size() << endl;
        cout << "  Total Revenue: Rs. " << fixed << setprecision(2);

        double totalRev = 0.0;
        for (const auto &ticket : tickets)
        {
            totalRev += ticket.getAmountPaid();
        }
        cout << totalRev << endl;

        int totalAvailable = 0;
        for (const auto &route : routes)
        {
            totalAvailable += route.getAvailableSeats();
        }
        cout << "  Total Available Seats: " << totalAvailable << endl;
        cout << "========================================\n"
             << endl;
    }
};

// ==================== MAIN FUNCTION ====================
/**
 * @brief Main entry point for the Bus Reservation System.
 *
 * Implements a menu-driven interface allowing users to:
 * 1. View available routes
 * 2. Book tickets
 * 3. Cancel tickets
 * 4. Search for tickets
 * 5. Generate reports
 * 6. Exit the system
 */
int main()
{
    // Disable output buffering for better WASM compatibility
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Initialize the reservation system
    ReservationSystem system("tickets.txt");

    cout << "\n";
    cout << "============================================================" << endl;
    cout << "         BUS RESERVATION & CANCELLATION SYSTEM              " << endl;
    cout << "                    Welcome!                                " << endl;
    cout << "============================================================" << endl;

    int choice = 0;
    while (true)
    {
        cout << "\n============================================================" << endl;
        cout << "                        MAIN MENU                            " << endl;
        cout << "============================================================" << endl;
        cout << " [1] Display Available Routes                               " << endl;
        cout << " [2] Book a Ticket                                          " << endl;
        cout << " [3] Cancel a Ticket                                        " << endl;
        cout << " [4] Search for a Ticket                                    " << endl;
        cout << " [5] Generate Reports                                       " << endl;
        cout << " [6] Display System Statistics                              " << endl;
        cout << " [7] Save and Exit                                          " << endl;
        cout << "============================================================" << endl;

        cout << "Enter your choice (1-7): ";
        cout.flush(); // Ensure prompt is displayed immediately

        // Input validation and error handling
        if (!(cin >> choice))
        {
            // Handle invalid input (non-numeric)
            cin.clear();
            cin.ignore(10000, '\n');
            cerr << "\nError: Please enter a valid number (1-7)!" << endl;
            continue;
        }
        cin.ignore(10000, '\n'); // Clear the entire line after reading number

        // Process user choice
        switch (choice)
        {
        case 1:
            // Display available routes
            system.displayRoutes();
            break;

        case 2:
            // Book a ticket
            system.bookTicket();
            break;

        case 3:
            // Cancel a ticket
            system.cancelTicket();
            break;

        case 4:
            // Search for a ticket
            system.searchTicket();
            break;

        case 5:
            // Generate reports
            system.generateReports();
            break;

        case 6:
            // Display statistics
            system.displayStatistics();
            break;

        case 7:
            // Save data and exit
            system.saveData();
            cout << "\n============================================================" << endl;
            cout << "                    Thank you for using BRS!                 " << endl;
            cout << "                      Goodbye!                               " << endl;
            cout << "============================================================" << endl;
            return 0;

        default:
            // Invalid menu choice
            cerr << "Error: Invalid choice! Please select a valid option (1-7)." << endl;
        }
    }

    return 0;
}
