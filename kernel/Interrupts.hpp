#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

struct InterruptFrame;
__attribute__((interrupt)) void PageFaultHandler(struct InterruptFrame* frame);

#endif // INTERRUPTS_H_
