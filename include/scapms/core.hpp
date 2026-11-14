#pragma once

#include <array>
#include <memory>
#include <optional>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

namespace scapms {

class ValidationError : public std::runtime_error { public: using std::runtime_error::runtime_error; };
enum class PatientType { General, Emergency, FollowUp };
enum class AppointmentStatus { Pending, Completed, Cancelled };
enum class SortField { Name, Id, Age };
enum class SortAlgorithm { Selection, Insertion };

struct PatientData { PatientType type; std::string id, name, contact; int age{}; };
struct Appointment { std::string id, patientId, date, reason; AppointmentStatus status{AppointmentStatus::Pending}; };

class Appointable {
public:
    virtual ~Appointable() = default;
    virtual void scheduleAppointment(const Appointment&) = 0;
    virtual void cancelAppointment(const std::string&) = 0;
};

class PatientRecord : public Appointable {
    PatientData data_;
    std::vector<std::string> appointmentIds_;
public:
    explicit PatientRecord(PatientData data);
    virtual ~PatientRecord() = default;
    const PatientData& data() const noexcept { return data_; }
    void update(PatientData data);
    void scheduleAppointment(const Appointment& a) override { appointmentIds_.push_back(a.id); }
    void cancelAppointment(const std::string& id) override;
    virtual std::string careLabel() const = 0;
    virtual std::unique_ptr<PatientRecord> clone() const = 0;
};
class GeneralPatient final : public PatientRecord {
public: using PatientRecord::PatientRecord; std::string careLabel() const override { return "Normal priority"; }
    std::unique_ptr<PatientRecord> clone() const override;
};
class EmergencyPatient final : public PatientRecord {
public: using PatientRecord::PatientRecord; std::string careLabel() const override { return "HIGH PRIORITY"; }
    std::unique_ptr<PatientRecord> clone() const override;
};
class FollowUpPatient final : public PatientRecord {
public: using PatientRecord::PatientRecord; std::string careLabel() const override { return "Follow-up"; }
    std::unique_ptr<PatientRecord> clone() const override;
};

class ClinicDatabase {
    std::vector<std::unique_ptr<PatientRecord>> patients_;
    std::vector<Appointment> appointments_;
    std::queue<std::string> waiting_;
public:
    struct Snapshot { std::vector<PatientData> patients; std::vector<Appointment> appointments; std::vector<std::string> waiting; };
    const auto& patients() const noexcept { return patients_; }
    const auto& appointments() const noexcept { return appointments_; }
    PatientRecord* findLinear(const std::string& id) const;
    PatientRecord* findBinaryId(const std::string& id) const;
    PatientRecord* findRecursiveName(const std::string& name) const;
    std::vector<PatientRecord*> findCategory(PatientType type) const;
    void addPatient(PatientData data);
    void editPatient(const std::string& id, PatientData data);
    void deletePatient(const std::string& id);
    void addAppointment(Appointment a);
    void cancelAppointment(const std::string& id);
    void completeAppointment(const std::string& id);
    void sortPatients(SortField field, SortAlgorithm algorithm);
    void enqueue(const std::string& patientId);
    std::optional<std::string> callNext();
    std::vector<std::string> queueItems() const;
    Snapshot snapshot() const;
    void restore(const Snapshot& snapshot);
    void save(const std::string& path) const;
    void load(const std::string& path);
    size_t pendingCount() const;
    size_t completedCount() const;
    size_t cancelledCount() const;
};

class ClinicManager {
    ClinicDatabase db_;
    std::stack<ClinicDatabase::Snapshot> undo_;
    std::array<std::optional<std::string>, 10> recent_{};
    size_t recentNext_{};
    unsigned nextPatient_{1}, nextAppointment_{1};
    void remember();
    void cache(const std::string& id);
public:
    ClinicDatabase& database() noexcept { return db_; }
    const ClinicDatabase& database() const noexcept { return db_; }
    std::string registerPatient(PatientType type, std::string name, int age, std::string contact);
    void editPatient(const std::string& id, PatientType type, std::string name, int age, std::string contact);
    void deletePatient(const std::string& id);
    std::string schedule(const std::string& patientId, std::string date, std::string reason);
    void cancelAppointment(const std::string& id);
    void completeAppointment(const std::string& id);
    void joinQueue(const std::string& patientId);
    std::optional<std::string> callNext();
    bool undo();
    void save(const std::string& path) const { db_.save(path); }
    void load(const std::string& path);
    std::vector<std::string> mostAttended(size_t limit = 10) const;
    std::vector<std::string> pendingAppointments() const;
    std::vector<std::string> categoryReport() const;
    std::vector<std::string> dailySummary(const std::string& date) const;
    std::vector<std::string> recentPatients() const;
};

std::string toString(PatientType type);
std::string toString(AppointmentStatus status);
PatientType patientTypeFromString(const std::string& s);
std::string generateId(const std::string& prefix, unsigned value);

} // namespace scapms
