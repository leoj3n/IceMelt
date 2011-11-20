/*
 * The base application class for IceMelt.
 *
 * Adapted from part of the Cyclone physics system.
 *
 * Now uses custom cyclone.h header and unused classes removed.
 *
 */
#include <cstdlib>

#include "cyclone.h"

/**
 * An application is the base class for all demonstration progams.
 * GLUT is a c-style API, which calls bare functions. This makes
 * it more difficult to provide default services for all demos and
 * only override them when needed.
 *
 * To solve this, the GLUT API is translated into calls on a
 * generic application object. Each demonstration will create a
 * concrete subclass of Application, providing the behaviours it
 * needs. The common code for all demos manages dispatch of
 * requests to the appropriate application object.
 *
 * To provide a correct application object of the right type without
 * the core code needing to know which subclass is being used, each
 * demonstration will supply a getApplication function which creates
 * (with new) and returns a pointer to a new Application instance.
 *
 * Even though subclasses will have to implement most of the methods
 * in this class, I have not made them pure virtual. This saves the
 * annoying need to implement an empty function that isn't needed.
 */
class Application
{
protected:
    /**
     * Holds the height of the application window.
     */
    int height;

    /**
     * Holds the current width of the application window.
     */
    int width;

public:
    /**
     * Gets the title of the demo for the title bar of the window.
     *
     * The default implementation returns a generic title.
     */
    virtual const char* getTitle();

    /**
     * Sets up the graphics, and allows the application to acquire
     * graphical resources. Guaranteed to be called after OpenGL is
     * set up.
     *
     * The default implementation sets up a basic view, and calls
     * setView to set up the camera projection.
     */
    virtual void initGraphics();

    /**
     * Called to set the projection characteristics of the camera.
     *
     * The default implementation uses a 60 degree field of view camera
     * with a range from 1-500 units.
     */
    virtual void setView();

    /**
     * Called just before the application is destroyed. Clear up can
     * be performed here or in the application destructor.
     *
     * The default implementation does nothing.
     */
    virtual void deinit();

    /**
     * Called each frame to display the current scene. The common code
     * will automatically flush the graphics pipe and swap the render
     * buffers after calling this so glFlush doesn't need to be called.
     *
     * The default
     * implementation draws a simple diagonal line across the surface
     * (as a sanity check to make sure GL is working).
     */
    virtual void display();

    /**
     * Called each frame to update the current state of the scene.
     *
     * The default implementation requests that the display be refreshed.
     * It should probably be called from any subclass update as the last
     * command.
     */
    virtual void update();

    /**
     * Called when a keypress is detected.
     *
     * The default implementation does nothing.
     *
     * @param key The ascii code of the key that has been pressed.
     */
    virtual void key(unsigned char key);


    /**
     * Notifies the application that the window has changed size.
     * The new size is given.
     *
     * The default implementation sets the internal height and width
     * parameters and changes the gl viewport. These are steps you'll
     * almost always need, so its worth calling the base class version
     * of this method even if you override it in a demo class.
     */
    virtual void resize(int width, int height);

    /**
     * Called when GLUT detects a mouse button press.
     *
     * The default implementation does nothing.
     */
    virtual void mouse(int button, int state, int x, int y);

    /**
     * Called when GLUT detects a mouse drag.
     *
     * The default implementation does nothing.
     */
    virtual void mouseDrag(int x, int y);

    // These are helper functions that can be used by an application 
    // to render things.

    /**
     * Renders the given text to the given x,y location (in screen space)
     * on the window. This is used to pass status information to the 
     * application.
     */
    void renderText(float x, float y, const char *text, void* font=NULL);
};
