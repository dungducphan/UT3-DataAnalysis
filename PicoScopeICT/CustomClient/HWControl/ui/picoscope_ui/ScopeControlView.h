#pragma once

#include <string>
#include <functional>

class ScopeManager;

class ScopeControlView {
public:
    // Constructor requires a reference to the ScopeManager (Model)
    explicit ScopeControlView(ScopeManager& cameraManager);

    // Main method to render the GUI
    void Render();

    // Event handlers for user interactions
    void SetOnAddScopeCallback(std::function<void(const std::string&, const std::string&, const std::string&)> callback);
    void SetOnRemoveScopeCallback(std::function<void(const std::string&)> callback);

private:
    // Reference to the ScopeManager (Model)
    ScopeManager& cameraManager_;

    // Callbacks for user actions
    std::function<void(const std::string&, const std::string&, const std::string&)> onAddScopeCallback_;
    std::function<void(const std::string&)> onRemoveScopeCallback_;

    // Helper methods to render GUI components
    void RenderAddScope();
    void RenderScopeList();
};