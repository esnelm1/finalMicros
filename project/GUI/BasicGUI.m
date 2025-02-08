classdef BasicGUI < matlab.apps.AppBase

    % Properties que contienen los componentes de la app
    properties (Access = public)
        UIFigure        matlab.ui.Figure
        Button          matlab.ui.control.Button
        EditFieldLabel  matlab.ui.control.Label
        EditField       matlab.ui.control.EditField
    end

    % Métodos de inicialización
    methods (Access = private)

        % Código que se ejecuta cuando el botón es presionado
        function onButtonPushed(app, event)
            userText = app.EditField.Value; % Obtiene el texto del cuadro de texto
            uialert(app.UIFigure, userText, 'Mensaje'); % Muestra una alerta con el texto
        end
    end

    % Métodos de creación de componentes
    methods (Access = private)

        % Crea todos los componentes de la app
        function createComponents(app)

            % Crear la ventana principal
            app.UIFigure = uifigure('Position', [450, 200, 800, 600]);
            app.UIFigure.Name = 'Basic GUI';

            % Crear el botón
            app.Button = uibutton(app.UIFigure, 'push');
            app.Button.Position = [100, 30, 100, 30];
            app.Button.Text = 'Presiona aquí';
            app.Button.ButtonPushedFcn = @(~, event) onButtonPushed(app, event);

            % Crear etiqueta para el cuadro de texto
            app.EditFieldLabel = uilabel(app.UIFigure);
            app.EditFieldLabel.HorizontalAlignment = 'right';
            app.EditFieldLabel.Position = [50, 90, 50, 22];
            app.EditFieldLabel.Text = 'Texto:';

            % Crear cuadro de texto
            app.EditField = uieditfield(app.UIFigure, 'text');
            app.EditField.Position = [110, 90, 150, 22];
        end
    end

    % Métodos de inicialización y terminación de la app
    methods (Access = public)

        % Constructor
        function app = BasicGUI
            % Crear y configurar los componentes
            createComponents(app)
        end

        % Destructor
        function delete(app)
            % Eliminar la ventana principal
            delete(app.UIFigure)
        end
    end
end
