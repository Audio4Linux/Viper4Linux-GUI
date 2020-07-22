#ifndef GSTREGISTRYHELPER_H
#define GSTREGISTRYHELPER_H
#include <gst/gst.h>
#include <gst/gstplugin.h>

class GstRegistryHelper
{
public:
    static const char* GetPluginPath(){
        GstPlugin* plg = gst_registry_find_plugin (gst_registry_get(), "viperfx");
        return gst_plugin_get_filename (plg);
    }

    static const char* GetPluginVersion(){
        GstPlugin* plg = gst_registry_find_plugin (gst_registry_get(), "viperfx");
        return (gst_plugin_get_version(plg));
    }

    static bool IsPluginInstalled(){
        return gst_registry_check_feature_version
                (gst_registry_get(), "viperfx", 1, 0, 0);;
    }

    static bool HasDBusSupport(){
        return gst_registry_check_feature_version
                (gst_registry_get(), "viperfx", 2, 0, 0);
    }

};

#endif // GSTREGISTRYHELPER_H
