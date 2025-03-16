#pragma once
class Station {
    private:
    int m_id;
    bool m_isOccupied = false;
    float m_timeOccupied = 0.0f;


    public:
    Station(int stationId) : m_id(stationId) {}

    int getId() const {
        return m_id;
    }

    void setState(bool newState) {
        m_isOccupied = newState;
    }

    void incrementTime(float dt) {
        if (m_isOccupied) {
            m_timeOccupied += dt;
        }
    }
};
