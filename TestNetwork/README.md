
# Test Network Layer

This is test distributed network  designed to test out the concept of having infrastrucvture for distributed API services to communicate and share data across a peer-to-peer network.

## Design

The design is loosely based on existing distributed networks, with multiple nodes connecting to one another with no central server.<br>

For the network to be initiated, a Genesis Node is required; a Genesis Node is simply the first node to be created that marks the start of tbe 'mesh' of connected nodes.<br><br>

Technically speaking; the Genesis Node is the only listening server to exist before a joining Node connects to it, converting it to a regular Node once the first connection has been established.

## Usage

Inside the `tests` directory is a test echo app written in Python. Once both nodes have been created and connected to one another, `helloResponse.py` is connected to one of the nodes via a socket connection and `helloWorld.py` is connected to another. <br><br>
The user can interact with `HelloWorld.py`; sending responses across the network to a neighboring node with the `helloResponse.py` application receiving response and returning it's own.<br>

### Example
#### Initialize the server
Terminal 1 (Genesis node opens itself up for listening):
```
./test_node -S 4242
```

Terminal 2 (Client node connects to Genesis node and opens a separate listening port on 4321):
```
./test_node -C 127.0.0.1 4242 
```

Right now, Terminal 1 is listening on port `4242` and Terminal 2 is listening on `4321`

#### Connect the apps
Terminal 3 (attach the app to node at Terminal 2)
```
python3 HelloResponse.py 4321
```
Terminal 4 (attach its counterpart to Terminal 1)
```
python3 HelloWorld.py 4242
```

#### Interact with the HelloWorld.py

```
Hello user$>Hello world
received I received Hello World
```
#### See the result on the helloResponse.py screen
```
receiving request from user Hello World
responding
to respond is  Hello World
```


# TODO
- Add security to the API layer.
- Support UDP.
- Test (a lot).

# Build
```
make all
```
