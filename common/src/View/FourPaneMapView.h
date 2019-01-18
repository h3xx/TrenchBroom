/*
 Copyright (C) 2010-2017 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_FourPaneMapView
#define TrenchBroom_FourPaneMapView

#include "View/CameraLinkHelper.h"
#include "View/MultiMapView.h"
#include "View/ViewTypes.h"

class QSplitter;

namespace TrenchBroom {
    class Logger;
    
    namespace Renderer {
        class MapRenderer;
    }
    
    namespace View {
        class GLContextManager;
        class MapViewBase;
        class MapView2D;
        class MapView3D;
        class MapViewToolBox;
        class SplitterWindow4;
        
        class FourPaneMapView : public MultiMapView {
        private:
            static const char* HSaveStateKey;
            static const char* VSaveStateKey;
        private:
            Logger* m_logger;
            MapDocumentWPtr m_document;
            
            CameraLinkHelper m_linkHelper;
            QSplitter* m_hSplitter;
            QSplitter* m_leftVSplitter;
            QSplitter* m_rightVSplitter;

            MapView3D* m_mapView3D;
            MapView2D* m_mapViewXY;
            MapView2D* m_mapViewXZ;
            MapView2D* m_mapViewYZ;
        public:
            FourPaneMapView(QWidget* parent, Logger* logger, MapDocumentWPtr document, MapViewToolBox& toolBox, Renderer::MapRenderer& mapRenderer, GLContextManager& contextManager);
            ~FourPaneMapView() override;
        private:
            void createGui(MapViewToolBox& toolBox, Renderer::MapRenderer& mapRenderer, GLContextManager& contextManager);
            void saveLayoutToPrefs();
        private: // event handlers
            void onSplitterMoved(int pos, int index);
        private: // implement MultiMapView subclassing interface
            void doMaximizeView(MapView* view) override;
            void doRestoreViews() override;
        };
    }
}

#endif /* defined(TrenchBroom_FourPaneMapView) */
