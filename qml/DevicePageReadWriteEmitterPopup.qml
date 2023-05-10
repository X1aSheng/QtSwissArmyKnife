import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ED.EasyDebug
import "common"

EDPopup {
    id: root

    readonly property string itemEnable: emitterTool.itemEnable
    readonly property string itemDescription: emitterTool.itemDescription
    readonly property string itemEscapeCharacter: emitterTool.itemEscapeCharacter
    readonly property string itemTextFormat: emitterTool.itemTextFormat
    readonly property string itemPrefix: emitterTool.itemPrefix
    readonly property string itemSuffix: emitterTool.itemSuffix
    readonly property string itemInterval: emitterTool.itemInterval
    readonly property string itemText: emitterTool.itemText
    readonly property string itemCrcEnable: emitterTool.itemCrcEnable
    readonly property string itemCrcAlgorithm: emitterTool.itemCrcAlgorithm
    readonly property string itemCrcStartIndex: emitterTool.itemCrcStartIndex
    readonly property string itemCrcEndIndex: emitterTool.itemCrcEndIndex

    contentItem:  ColumnLayout {
        GridLayout {
            Layout.fillWidth: true
            columns: 2
            EDCheckBox {
                id: itemEnableCheckBox
                text: qsTr("Enable the item")
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            EDLabel {
                text: qsTr("Description")
            }
            EDTextField {
                id: itemDescriptionTextField
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("Item text format")
                Layout.fillWidth: true
            }
            EDTextFormatComboBox {
                id: itemTextFormatComboBox
                Layout.fillWidth: true
                onActivated: itemTextTextField.clear()
            }
            EDLabel {
                text: qsTr("Item escape")
                Layout.fillWidth: true
            }
            EDEscComboBox {
                id: itemEscapeCharacterComboBox
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("Item prefix")
                Layout.fillWidth: true
            }
            EDEscComboBox {
                id: itemPrefixComboBox
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("Item suffix")
                Layout.fillWidth: true
            }
            EDEscComboBox {
                id: itemSuffixComboBox
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("Item interval")
                Layout.fillWidth: true
            }
            EDSpinBox {
                id: itemIntervalSpinBox
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("Item text")
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            EDTextField {
                id: itemTextTextField
                validator: itemTextFormatComboBox.currentValidator
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                Layout.columnSpan: 2
            }
            EDCheckBox {
                id: itemCrcEnableCheckBox
                text: qsTr("Append CRC data")
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            EDLabel {
                text: qsTr("CRC arithmetic")
            }
            EDCrcComboBox {
                id: itemCrcArithmeticComboBox
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("CRC start index")
            }
            EDSpinBox {
                id: itemCrcStartIndexSpinBox
                Layout.fillWidth: true
            }
            EDLabel {
                text: qsTr("CRC end index")
            }
            EDSpinBox {
                id: itemCrcEndIndexSpinBox
                Layout.fillWidth: true
            }
        }
        Row {
            spacing: 8
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Repeater {
                model: [qsTr("Cancle"), qsTr("Apply")]
                EDButton {
                    text: modelData
                    width: Math.round((parent.width - parent.spacing)/2)
                    onClicked: {
                        if (index === 0) {
                            root.rejected()
                        } else {
                            root.accepted()
                        }
                    }
                }
            }
        }
    }

    function setParameters(parameters) {
        if (parameters) {
            itemEnableCheckBox.checked = parameters[itemEnable]
            itemDescriptionTextField.text = parameters[itemDescription]
            var ret = itemTextFormatComboBox.indexOfValue(parameters[itemTextFormat])
            if (ret >= 0) {
                itemTextFormatComboBox.currentIndex = ret
            }
            ret = itemEscapeCharacterComboBox.indexOfValue(parameters[itemEscapeCharacter])
            if (ret >= 0) {
                itemEscapeCharacterComboBox.currentIndex = ret
            }
            ret = itemPrefixComboBox.indexOfValue(parameters[itemPrefix])
            if (ret >= 0) {
                itemPrefixComboBox.currentIndex = ret
            }
            ret = itemSuffixComboBox.indexOfValue(parameters[itemSuffix])
            if (ret >= 0) {
                itemSuffixComboBox.currentIndex = ret
            }
            itemIntervalSpinBox.value = parameters[itemInterval]
            itemTextTextField.text = parameters[itemText]

            itemCrcEnableCheckBox.checked = parameters[itemCrcEnable]
            ret = itemCrcArithmeticComboBox.indexOfValue(parameters[itemCrcAlgorithm])
            if (ret >= 0) {
                itemCrcArithmeticComboBox.currentIndex = ret
            }
            itemCrcStartIndexSpinBox.value = parameters[itemCrcStartIndex]
            itemCrcEndIndexSpinBox.value = parameters[itemCrcEndIndex]
        }
    }

    function getParameters() {
        var parameters = {}
        parameters[itemEnable] = itemEnableCheckBox.checked
        parameters[itemDescription] = itemDescriptionTextField.text
        parameters[itemEscapeCharacter] = itemEscapeCharacterComboBox.currentValue
        parameters[itemTextFormat] = itemTextFormatComboBox.currentValue
        parameters[itemPrefix] = itemPrefixComboBox.currentValue
        parameters[itemSuffix] = itemSuffixComboBox.currentValue
        parameters[itemTextFormat] = itemTextFormatComboBox.currentValue
        parameters[itemInterval] = itemIntervalSpinBox.value
        parameters[itemText] = itemTextTextField.text

        parameters[itemCrcEnable] = itemCrcEnableCheckBox.checked
        parameters[itemCrcAlgorithm] = itemCrcArithmeticComboBox.currentValue
        parameters[itemCrcStartIndex] = itemCrcStartIndexSpinBox.value
        parameters[itemCrcEndIndex] = itemCrcEndIndexSpinBox.value
        return parameters
    }
}