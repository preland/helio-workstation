/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "DialogBase.h"
#include "Meter.h"
#include "TimeSignatureEvent.h"
#include "MobileComboBox.h"
#include "UndoStack.h"
#include "TimeSignaturesSequence.h"
#include "MidiTrack.h"

class ProjectNode;
class MetronomeEditor;

class TimeSignatureDialog final : public DialogBase
{
public:

    TimeSignatureDialog(Component &owner, ProjectNode &project,
        WeakReference<MidiTrack> targetTrack,
        WeakReference<TimeSignaturesSequence> targetSequence,
        const TimeSignatureEvent &editedEvent,
        bool shouldAddNewEvent);

    ~TimeSignatureDialog();

    static UniquePointer<Component> editingDialog(Component &owner,
        ProjectNode &project, const TimeSignatureEvent &event);

    static UniquePointer<Component> addingDialog(Component &owner,
        ProjectNode &project, WeakReference<TimeSignaturesSequence> tsSequence, float targetBeat);

    void resized() override;
    void parentHierarchyChanged() override;
    void parentSizeChanged() override;
    void handleCommandMessage(int commandId) override;
    void inputAttemptWhenModal() override;

private:

    const WeakReference<UndoStack> undoStack;

    // either of them will be nullptr:
    const WeakReference<MidiTrack> targetTrack;
    const WeakReference<TimeSignaturesSequence> targetSequence;

    // keeping a copy of original event so we can send undo actions (from -> to):
    TimeSignatureEvent originalEvent;
    // all current edits are here so callbacks can just modify them:
    TimeSignatureEvent editedEvent;

    Component &ownerComponent;

    const Array<Meter::Ptr> defaultMeters;

    inline void undoAndDismiss();
    inline void updateOkButtonState();
    void updateSize();

    enum class Mode
    {
        EditTrackTimeSignature,
        EditTimelineTimeSignature,
        AddTimelineTimeSignature
    };

    const Mode mode;

    bool hasMadeChanges = false;

    void sendEventChange(const TimeSignatureEvent &newEvent);
    void removeTimeSignature();

    UniquePointer<MobileComboBox::Container> presetsCombo;
    UniquePointer<Label> messageLabel;
    UniquePointer<TextButton> removeEventButton;
    UniquePointer<TextButton> okButton;
    UniquePointer<TextEditor> textEditor;
    UniquePointer<MetronomeEditor> metronomeEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeSignatureDialog)
};
