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

    // TODO: This doesn't seem to be need. Keep for now.
    // 
    //  AudioThreadAutomationModifier
    // 
    //  struct AudioThreadAutomationModifier::AudioThreadAutomationModifierTimer : public ModifierTimer
    //  {
    //      AudioThreadAutomationModifierTimer(AudioThreadAutomationModifier& tm)
    //      : modifier(tm)
    //      {}
    //
    //      void updateStreamTime(tracktion::core::TimePosition editTime, int numSamples) override
    //      {}
    //
    //      AudioThreadAutomationModifier& modifier;
    //
    //      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioThreadAutomationModifierTimer)
    //  };
    // 
    /////////////////////////////////////////////////////////////////////////////////////////////

    AudioThreadAutomationModifier::Assignment::Assignment (const juce::ValueTree& v, const AudioThreadAutomationModifier& tm)
        : AutomatableParameter::ModifierAssignment (tm.edit, v), m_AudioThreadAutomationModifierID (tm.itemID)
    {
    }

    bool AudioThreadAutomationModifier::Assignment::isForModifierSource (const ModifierSource& source) const
    {
        if (auto* mod = dynamic_cast<const AudioThreadAutomationModifier*> (&source))
            return mod->itemID == m_AudioThreadAutomationModifierID;

        return false;
    }

    AudioThreadAutomationModifier::Ptr AudioThreadAutomationModifier::Assignment::getModifier() const
    {
        if (auto mod = findModifierTypeForID<AudioThreadAutomationModifier> (edit, m_AudioThreadAutomationModifierID))
            return mod;

        return {};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////

    AudioThreadAutomationModifier::AudioThreadAutomationModifier (Edit& e, const juce::ValueTree& v)
        : Modifier (e, v)
    {
        // TODO: This doesn't seem to be need. Keep for now.
        //
        //  m_ChangedTimer.setCallback(
        //      [this]
        //      {
        //          m_ChangedTimer.stopTimer();
        //          changed();
        //      });

        state.addListener (this);
    }

    AudioThreadAutomationModifier::~AudioThreadAutomationModifier()
    {
        state.removeListener (this);

        notifyListenersOfDeletion();

        // TODO: This doesn't seem to be need. Keep for now.
        //
        // edit.removeModifierTimer(*m_ModifierTimer);

        for (auto p : getAutomatableParameters())
            p->detachFromCurrentValue();

        deleteAutomatableParameters();
    }

    juce::String AudioThreadAutomationModifier::getName() const
    {
        return juce::String();
    }

    juce::String AudioThreadAutomationModifier::getSelectableDescription()
    {
        return juce::String();
    }

    void AudioThreadAutomationModifier::initialise()
    {
        // TODO: This doesn't seem to be need. Keep for now.
        //
        //  m_ModifierTimer = std::make_unique<AudioThreadAutomationModifierTimer>(*this);
        //  edit.addModifierTimer(*m_ModifierTimer);

        restoreChangedParametersFromState();
    }

    float AudioThreadAutomationModifier::getCurrentValue()
    {
        return m_CurrentValue.load (std::memory_order_acquire);
    }

    AutomatableParameter::ModifierAssignment* AudioThreadAutomationModifier::createAssignment (const juce::ValueTree& v)
    {
        return new Assignment (v, *this);
    }

    void AudioThreadAutomationModifier::applyToBuffer (const PluginRenderContext&)
    {
        m_CurrentValue.store (m_TransitionValue, std::memory_order_release);
    }

    void AudioThreadAutomationModifier::valueTreeChanged()
    {
        // TODO: This doesn't seem to be need. Keep for now.
        //
        //  if (!m_ChangedTimer.isTimerRunning())
        //      m_ChangedTimer.startTimerHz(100);
    }
} // namespace engine
} // namespace tracktion