from openai import OpenAI

client = OpenAI(
  base_url = "https://integrate.api.nvidia.com/v1",
  api_key = "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"
)

completion = client.chat.completions.create(
  model="nvidia/nvidia-nemotron-nano-9b-v2",
  messages=[{"role":"user","content":""}],
  temperature=0.6,
  top_p=0.95,
  max_tokens=2048,
  frequency_penalty=0,
  presence_penalty=0,
  stream=False,
  extra_body={
    "min_thinking_tokens": 1024,
    "max_thinking_tokens": 2048
  }
)

reasoning = getattr(completion.choices[0].message, "reasoning_content", None)
if reasoning:
  print(reasoning)
print(completion.choices[0].message.content)
