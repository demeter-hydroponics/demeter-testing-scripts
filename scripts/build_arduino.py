import subprocess
import os
import argparse


def main(deploy: bool = False, monitor: bool = False):

    # Run the build script with platformio
    subprocess.run("pio run", shell=True, check=True)
    # If deploy is True, run the upload script
    if deploy:
        subprocess.run(
            "pio run --target upload", shell=True, check=True
        )
    if monitor:
        subprocess.run(
            "pio device monitor", shell=True, check=True
        )


if __name__ == "__main__":
    # Argument parser to handle deploy option
    parser = argparse.ArgumentParser(
        description="Build and optionally deploy the project."
    )
    parser.add_argument(
        "--deploy",
        action="store_true",
        help="Deploy the project to the connected device",
    )

    parser.add_argument(
        "--monitor",
        action="store_true",
        help="Monitor the serial output of the device",
    )

    args = parser.parse_args()

    os.chdir("aio-test")

    main(args.deploy, args.monitor)

# if monitoring is ever needed run pio device monitor