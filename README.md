# Smart-Hospital-Patient-Resource-Allocation-System

A modular, menu-driven C application designed to simulate daily hospital patient intake, bed occupancy tracking, emergency triage sorting, and automated billing calculation. Developed for the **CSC 1012: Introduction to Computer Programming** course at the University of Sri Jayewardenepura.

---

## 📌 Features & Capabilities

- **Lookup Data Tables:** Pre-configured data arrays for Doctor Specialties (Consultation fees, avg. times, caps) and Hospital Wards (Daily rates, bed capacities).
- **Bed Occupancy Tracking:** 2D Array matrix (`int bedOccupancy[4][20]`) to track real-time bed availability (0 = Available, 1 = Occupied)[cite: 1].
- **Patient Intake & Registration:** Collects patient demographics, urgency levels, specialty choices, and ward stay requirements[cite: 1].
- **Automated Billing Engine:** 
  - Dynamic waiting time estimation[cite: 1].
  - Emergency surcharge calculation (Level 1: 0%, Level 2: 20%, Level 3: 50%)[cite: 1].
  - Senior / Child subsidy discount (15% for Age < 5 or > 65)[cite: 1].
- **Emergency Triage Priority Sorting:** Sorts and displays patients by urgency level (Critical Level 3 cases first)[cite: 1].
- **Performance Analytics Report:** Displays total revenue, discounts granted, ward bed occupancy rates, and highest-paying patient records[cite: 1].

---

## ⚙️ Mathematical & Billing Logic

| Calculation | Formula / Logic |
| :--- | :--- |
| **Wait Time** | $\text{Current Queue} \times \text{Avg Time per Patient}$[cite: 1] |
| **Emergency Surcharge** | `0%` (Normal), `20%` (Urgent), `50%` (Critical) of Base Fee[cite: 1] |
| **Total Ward Cost** | $\text{Days Admitted} \times \text{Ward Rate}$[cite: 1] |
| **Gross Total** | $\text{Base Fee} + \text{Emergency Surcharge} + \text{Ward Cost}$[cite: 1] |
| **Age Subsidy** | 15% discount on Gross Total if $\text{Age} < 5$ or $\text{Age} > 65$[cite: 1] |
| **Final Payable** | $\text{Gross Total} - \text{Discount}$[cite: 1] |

---

## 🛠️ System Requirements & Setup

### Prerequisites
- GCC Compiler (MinGW / GCC on Linux / Clang on macOS)
- Terminal or any C IDE (VS Code, Code::Blocks, CLion)

### How to Build and Run

1. **Clone the Repository:**
   ```bash
   git clone [https://github.com/YOUR_USERNAME/YOUR_REPOSITORY_NAME.git](https://github.com/YOUR_USERNAME/YOUR_REPOSITORY_NAME.git)
   cd YOUR_REPOSITORY_NAME

👤 Author Information
- Name: Tharuk Nadawa Rubasinghe Gunawardana
- Index Number: AS20250475
