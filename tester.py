import asyncio
import aiohttp
import random

async def stress_test(url: str, num_clients: int):
    sessions = [aiohttp.ClientSession() for _ in range(num_clients)]

    tasks = [asyncio.create_task(send_requests(session, url)) for session in sessions]

    await asyncio.gather(*tasks)

    for session in sessions:
        session.close()

async def send_requests(session: aiohttp.ClientSession, url: str):
    while True:
        method = random.choice(["GET", "POST"])

        if method == "GET":
            async with session.get(url) as response:
                print(f"Received response from {url}: {response.status}")
        else:
            data = {"value": random.randint(1, 100)}
            async with session.post(url, data=data) as response:
                print(f"Received response from {url}: {response.status}")
        if random.random() < 0.1:
            try:
                async with session.get(url + "/invalid"):
                    pass
            except aiohttp.client_exceptions.ClientResponseError as error:
                print(f"Received error from {url}: {error}")

if __name__ == "__main__":
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    url = "http://localhost:8080/"
    num_clients = 1000
    asyncio.run(stress_test(url, num_clients))

