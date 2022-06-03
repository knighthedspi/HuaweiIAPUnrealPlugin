using System.IO;
using Tools.DotNETCommon;

namespace UnrealBuildTool.Rules
{
    public class HuaweiIAP : ModuleRules
    {
        public HuaweiIAP(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivateIncludePaths.AddRange(
                new string[] {
                    "HuaweiIAP/Private"
                });

            PublicDependencyModuleNames.AddRange(
                new string[] {
                    "Core",
                    "CoreUObject",
                    "Engine",
					"RenderCore",
					"RHI"
				});

            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Settings",
                    "Launch"
                }
            );   

            if(Target.Platform == UnrealTargetPlatform.Android)
            {
                PublicDependencyModuleNames.AddRange(
                    new string[]
                    {
                        "Launch"
                    })
                ;
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(PluginPath, "HuaweiIAP_APL.xml")));
            }
        }
    }   
}