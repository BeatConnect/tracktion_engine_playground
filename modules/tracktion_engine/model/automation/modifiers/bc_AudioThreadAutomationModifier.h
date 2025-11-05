/////////////////////////////////////////////////////////////////////////////////////////////
//     ____             _    ____                            _                             //
//    | __ )  ___  __ _| |_ / ___|___  _ __  _ __   ___  ___| |_                           //
//    |  _ \ / _ \/ _` | __| |   / _ \| '_ \| '_ \ / _ \/ __| __|     Copyright 2020       //
//    | |_) |  __/ (_| | |_| |__| (_) | | | | | | |  __/ (__| |_    www.beatconnect.com    //
//    |____/ \___|\__,_|\__|\____\___/|_| |_|_| |_|\___|\___|\__|                          //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

namespace tracktion
{
inline namespace engine
{

    class AudioThreadAutomationModifier final : public Modifier, private ValueTreeAllEventListener
    {
    public:
        AudioThreadAutomationModifier (Edit&, const juce::ValueTree&);
        ~AudioThreadAutomationModifier() override;

        using Ptr = juce::ReferenceCountedObjectPtr<AudioThreadAutomationModifier>;
        using Array = juce::ReferenceCountedArray<AudioThreadAutomationModifier>;

        // Pure virtual inherited via Modifier
        juce::String getName() const override;
        juce::String getSelectableDescription() override;
        void initialise() override;
        float getCurrentValue() override;

        AutomatableParameter::ModifierAssignment* createAssignment (const juce::ValueTree&) override;

        struct Assignment : public AutomatableParameter::ModifierAssignment
        {
            Assignment (const juce::ValueTree&, const AudioThreadAutomationModifier&);

            bool isForModifierSource (const ModifierSource&) const override;

            juce::ReferenceCountedObjectPtr<AudioThreadAutomationModifier> getModifier() const;

            const EditItemID m_AudioThreadAutomationModifierID;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Assignment)
        };

        ProcessingPosition getProcessingPosition() override { return ProcessingPosition::preFX; }

        void applyToBuffer (const PluginRenderContext&) override;

        // Pure virtual inherited via ValueTreeAllEventListener
        void valueTreeChanged() override;

        std::atomic<float> m_TransitionValue { 0.0f };
        std::atomic<float> m_CurrentValue { 0.0f };

    private:
        // TODO: This doesn't seem to be need. Keep for now.
        //
        //	LambdaTimer m_ChangedTimer;
        //	struct AudioThreadAutomationModifierTimer;
        //	std::unique_ptr<AudioThreadAutomationModifierTimer> m_ModifierTimer;
    };

} // namespace engine
} // namespace tracktion