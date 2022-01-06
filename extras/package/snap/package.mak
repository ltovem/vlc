snap:
	export SNAPCRAFT_BUILD_INFO=1
    #snapcraft only support to be called from project root
	cd ../../.. && ln -s -f extras/package/snap/snapcraft.yaml .snapcraft.yaml
	cd ../../.. && snapcraft pack

snap-clean:
	cd ../../.. && snapcraft clean
	rm -rf snap
