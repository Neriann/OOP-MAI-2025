#pragma once

class NPC;

struct IFightObserver
{
    virtual ~IFightObserver() = default;

    virtual void on_fight(const NPC& attacker, const NPC& defender, bool win) = 0;
};

// Text Observer
struct TextObserver : IFightObserver {

    explicit TextObserver(const std::string& filename = "log.txt");

    static std::shared_ptr<IFightObserver> get(const std::string& filename = "log.txt");

    void on_fight(const NPC& attacker, const NPC& defender, bool win) override;

    ~TextObserver() override;

private:
    std::ostream& console_stream;
    std::ofstream file_stream;
};
